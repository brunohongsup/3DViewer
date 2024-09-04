#pragma once

#include "RavidQuadrangle.h"
#include "Matrix.h"

#include <memory>

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;

		class CInternalQRCodeDecoder;
		
		/**
		* 이미지의 2차원 QR코드를 해석하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CQRCodeDecoder : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CQRCodeDecoder);
		public:
			CQRCodeDecoder();
			virtual ~CQRCodeDecoder();

			/**
			* 배경대비 코드의 색 및 방법을 결정합니다.
			*/
			enum ECodeColor
			{
				ECodeColor_Black = 0,
				ECodeColor_White,
				ECodeColor_Auto_BlackToWhite,
				ECodeColor_Auto_WhiteToBlack,
			};
			/**
			* 코드의 방향 반전을 결정합니다.
			*/
			enum EUseFlip
			{
				EUseFlip_No = 0,
				EUseFlip_Yes,
				EUseFlip_Auto,
			};
			/**
			* 코드를 찾기위한 이진화 알고리즘입니다.
			*/
			enum EThresholdMethod
			{
				EThresholdMethod_Otsu = 0x01,
				EThresholdMethod_ISOData = 0x02,
				EThresholdMethod_MaximumEntropy = 0x04,
				EThresholdMethod_BlockLocalOtsuWithTuning = 0x08,
				EThresholdMethod_None = 0b01 << 4,
			};
			/**
			* 코드 에러 수정 기능을 결정합니다.
			*/
			enum EErrorCorrectingLevel
			{
				EErrorCorrectingLevel_None = -1,
				EErrorCorrectingLevel_L = 1,
				EErrorCorrectingLevel_M = 0,
				EErrorCorrectingLevel_Q = 3,
				EErrorCorrectingLevel_H = 2,
			};
			/**
			* 코드 모델을 결정합니다.
			*/
			enum EModel
			{
				EModel_None = 0,
				EModel_1 = 1,
				EModel_2 = 2,
			};
			/**
			* 코드 버전을 결정합니다.
			*/
			enum EVersion
			{
				EVersion_Auto = 0,
				EVersion_01 = 1,
				EVersion_02 = 2,
				EVersion_03 = 3,
				EVersion_04 = 4,
				EVersion_05 = 5,
				EVersion_06 = 6,
				EVersion_07 = 7,
				EVersion_08 = 8,
				EVersion_09 = 9,
				EVersion_10 = 10,
				EVersion_11 = 11,
				EVersion_12 = 12,
				EVersion_13 = 13,
				EVersion_14 = 14,
				EVersion_15 = 15,
				EVersion_16 = 16,
				EVersion_17 = 17,
				EVersion_18 = 18,
				EVersion_19 = 19,
				EVersion_20 = 20,
				EVersion_21 = 21,
				EVersion_22 = 22,
				EVersion_23 = 23,
				EVersion_24 = 24,
				EVersion_25 = 25,
				EVersion_26 = 26,
				EVersion_27 = 27,
				EVersion_28 = 28,
				EVersion_29 = 29,
				EVersion_30 = 30,
				EVersion_31 = 31,
				EVersion_32 = 32,
				EVersion_33 = 33,
				EVersion_34 = 34,
				EVersion_35 = 35,
				EVersion_36 = 36,
				EVersion_37 = 37,
				EVersion_38 = 38,
				EVersion_39 = 39,
				EVersion_40 = 40,
				EVersion_None = 41,
			};
			/**
			* QR 코드의 해석 결과 구조체입니다.
			*/
			typedef struct AFX_EXT_CLASS sQRCodeResult
			{
				CRavidQuadrangle<double> rqRegion;
				std::vector<CRavidQuadrangle<double>> vctRegionGridType;
				CRavidQuadrangle<double> rqrFindPattern[3];
				Ravid::Mathematics::CMatrix rmMatrix;

				EErrorCorrectingLevel eeclEcLevel;
				EModel emModel;
				EVersion evVersion;
				CString strText;

				sQRCodeResult() { Clear(); }
				virtual ~sQRCodeResult() { Clear(); }

				void Clear();
			}
			SQRCodeResult;
			
			/**
			* 해석 데이터를 초기화 합니다. 
			*/
			void Clear();
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Read(_In_ CRavidImage* pImgInfo);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Read(_In_ CRavidImage& refImgInfo);

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
			EAlgorithmResult Read(_In_ CRavidImage* pImgInfo, _In_opt_ CRavidRect<double>* pGeometry);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Reference)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Read(_In_ CRavidImage& refImgInfo, _In_opt_ CRavidRect<double>* pGeometry);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 배경대비 코드의 색 및 방법을 결정합니다.
			*
			* param[in] eCodeColor : 결정 enum
			*	ECodeColor_Black : 코드 검정색 만 탐색
			*	ECodeColor_White : 코드 흰색 만 탐색
			*	ECodeColor_Auto_BlackToWhite : 코드 검정색 -> 흰색 순서로 탐색
			*	ECodeColor_Auto_WhiteToBlack : 코드 흰색 -> 검정색 순서로 탐색
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetCodeColor(_In_ ECodeColor eCodeColor);
			
			/**
			* 설정된 배경대비 코드의 색 및 방법을 가져옵니다.
			*
			* return value : ECodeColor enum 반환합니다.
			*	ECodeColor_Black : 코드 검정색 만 탐색
			*	ECodeColor_White : 코드 흰색 만 탐색
			*	ECodeColor_Auto_BlackToWhite : 코드 검정색 -> 흰색 순서로 탐색
			*	ECodeColor_Auto_WhiteToBlack : 코드 흰색 -> 검정색 순서로 탐색
			*/
			_Out_ ECodeColor GetCodeColor();
			
			/**
			* 분석을 실행 전 사용합니다.
			* 찾을 객체의 방향 반전을 설정합니다.
			*
			* param[in] eFlippingRegion : 동작 방법 enum
			*	EUseFlip_No : 정방향
			*	EUseFlip_Yes : 반전
			*	EUseFlip_Auto : 자동
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetUseFlip(_In_ EUseFlip eFlip);
			
			/**
			* 설정된 찾을 객체의 방향 반전을 가져옵니다.
			*
			* return value : EUseFlip enum 반환합니다.
			*	EUseFlip_No : 정방향
			*	EUseFlip_Yes : 반전
			*	EUseFlip_Auto : 자동
			*/
			_Out_ EUseFlip GetUseFlip();
			
			/**
			* 분석을 실행 전 사용합니다.
			* 입력된 이진화 방법 사용 여부를 설정합니다.
			*
			* param[in] eDMTMO : EThresholdMethod enum
			*	EThresholdMethod_Ostu = 0x1
			*	EThresholdMethod_Adaptive1 = 0x2
			*	EThresholdMethod_Adaptive2 = 0x4
			*	EThresholdMethod_ISODATA = 0x8
			*	EThresholdMethod_MaximumEntropy = 0x10
			* param[in] bUse : 해당 알고리즘 사용 여부
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetThresholdMethod(_In_ CQRCodeDecoder::EThresholdMethod eMethod, _In_ bool bUse);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 입력된 이진화 방법 사용 여부를 확인합니다.
			*
			* param[in] eDMTMO : EThresholdMethod enum
			*	EThresholdMethod_Ostu = 0x1
			*	EThresholdMethod_Adaptive1 = 0x2
			*	EThresholdMethod_Adaptive2 = 0x4
			*	EThresholdMethod_ISODATA = 0x8
			*	EThresholdMethod_MaximumEntropy = 0x10
			* param[out] bUseOut : 해당 알고리즘 사용 여부 출력
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetThresholdMethod(_In_ CQRCodeDecoder::EThresholdMethod eMethod, _Out_ bool& bUseOut);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 객체 해석시 가우시안 필터 사용여부를 설정합니다.
			*
			* param[in] bUse : 설정 On/ Off
			*/
			void SetUseGaussianBlur(_In_ bool bUse);
			
			/**
			* 설정된 객체 해석시 가우시안 필터 사용여부를 가져옵니다.
			*
			* return value : bool 형태로 반환합니다.
			*/
			_Out_ bool GetUseGaussianBlur();
			
			/**
			* 분석을 실행 전 사용합니다.
			* 객체 해석시 코드 버전을 설정합니다.
			*
			* param[in] eVersion : 코드 입력 버전
			*/
			EAlgorithmResult SetStickVersion(_In_ CQRCodeDecoder::EVersion eVersion);
			
			/**
			* 객체 해석시 코드 버전을 가져옵니다.
			*
			* return value : EVersion 형태로 코드 입력 버전을 반환합니다.
			*/
			_Out_ EVersion GetStickVersion();

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
			_Out_ bool GetCleanImage(void);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 해석된 코드의 개수를 가져옵니다.
			*
			* return value : size_t 로 반환됩니다.
			*/
			size_t GetReadResultCount();
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 인식된 위치를 가져옵니다.
			*
			* param[out] refResult : 인식된 위치 (Reference)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetReadOutLine(_Out_ CRavidQuadrangle<double>& refResult, _In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 인식된 위치를 가져옵니다.
			*
			* param[out] refResult : 인식된 위치 (Pointer)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetReadOutLine(_Out_ CRavidQuadrangle<double>* pResult, _In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 코드 영역별 결과를 가져옵니다.
			*
			* param[out] refVctResult : 영역별 결과 위치 (Reference)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetGridTypeOfReadRegion(_Out_ std::vector<CRavidQuadrangle<double>>& refVctResult, _In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 코드 영역별 결과를 가져옵니다.
			*
			* param[out] pVctResult : 영역별 결과 위치 (Pointer)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetGridTypeOfReadRegion(_Out_ std::vector<CRavidQuadrangle<double>>* pVctResult, _In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 코드 영역 결과를 가져옵니다.
			*
			* param[out] rm : 코드 내부 인식 값 (Reference)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetReadMatrix(_Out_ Ravid::Mathematics::CMatrix& rm, _In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 코드 영역 결과를 가져옵니다.
			*
			* param[out] pRm : 코드 내부 인식 값 (Pointer)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetReadMatrix(_Out_ Ravid::Mathematics::CMatrix* pRm, _In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 코드 버전을 가져옵니다.
			*
			* param[out] evResult : 코드 버전 (Reference)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetReadVersion(_Out_ EVersion& evResult, _In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 코드 버전을 가져옵니다.
			*
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 코드 버전을 반환합니다.
			*/
			_Out_ EVersion GetReadVersion(_In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 코드 모델을 가져옵니다.
			*
			* param[out] evResult : 코드 모델 (Reference)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetReadModel(_Out_ EModel& emResult, _In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 코드 모델을 가져옵니다.
			*
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 코드 모델을 반환합니다.
			*/
			_Out_ EModel GetReadModel(_In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 에러 수정 레벨을 가져옵니다.
			*
			* param[out] evResult : 에러 수정 레벨 (Reference)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetReadEcLevel(_Out_ EErrorCorrectingLevel& eclResult, _In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 에러 수정 레벨을 가져옵니다.
			*
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 에러 수정 레벨을 반환합니다.
			*/
			_Out_ EErrorCorrectingLevel GetReadEcLevel(_In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 해석된 문자열을 가져옵니다.
			*
			* param[out] strResult : 해석 문자열 (Reference)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetReadString(_Out_ CString& strResult, _In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 해석된 문자열을 가져옵니다.
			*
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 해석 문자열을 반환합니다.
			*/
			_Out_ CString GetReadString(_In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 해석된 결과를 가져옵니다.
			*
			* param[out] sqrcResult : SQRCodeResult 결과 (Reference)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetReadResultAt(_Out_ SQRCodeResult& sqrcResult, _In_ int nIndex = 0);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 입력된 인덱스의 해석된 결과를 가져옵니다.
			*
			* param[out] pSqrcResult : SQRCodeResult 결과 (Pointer)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetReadResultAt(_Out_ SQRCodeResult* pSqrcResult, _In_ int nIndex = 0);
						
			/**
			* 분석을 실행 후 사용합니다.
			* 해석된 결과 리스트를 가져옵니다.
			*
			* param[out] vctResult : SQRCodeResult 결과 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetReadAllResult(_Out_ std::vector<SQRCodeResult>& vctResult);
						
			/**
			* 분석을 실행 후 사용합니다.
			* 해석된 결과 리스트를 가져옵니다.
			*
			* param[out] pVctResult : SQRCodeResult 결과 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetReadAllResult(_Out_ std::vector<SQRCodeResult>* pVctResult);

		private:
			std::unique_ptr<CInternalQRCodeDecoder> m_pInternal;

		};
	}
}