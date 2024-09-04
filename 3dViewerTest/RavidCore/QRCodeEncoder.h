#pragma once

#include <memory>

namespace Ravid
{
	namespace Algorithms
	{
		class CInternalQRCodeEncoder;
		
		/**
		* 이미지로 2차원 QR코드를 생성하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CQRCodeEncoder : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CQRCodeEncoder);
		public:
			CQRCodeEncoder();
			virtual ~CQRCodeEncoder();

			/**
			* QR 표현을 결정합니다.
			*/
			enum EModeIndicator
			{
				EModeIndicator_ECI = 0b0111, // 7
				EModeIndicator_Numeric = 0b0001, // 1
				EModeIndicator_Alphanumeric = 0b0010, // 2
				EModeIndicator_8bitByte = 0b0100, // 4
				EModeIndicator_Kanji = 0b1000, // 8
				EModeIndicator_StructuredAppend = 0b0011, // 3
				EModeIndicator_FNC1_FirstPos = 0b0101, // 5
				EModeIndicator_FNC1_SecondPos = 0b1001, // 9
				EModeIndicator_Terminator = 0b0000, // 0
			};
			/**
			* 코드 마스킹 패턴을 결정합니다.
			*/
			enum EMaskingPattern
			{
				EMaskingPattern_0 = 0,
				EMaskingPattern_1,
				EMaskingPattern_2,
				EMaskingPattern_3,
				EMaskingPattern_4,
				EMaskingPattern_5,
				EMaskingPattern_6,
				EMaskingPattern_7,
				EMaskingPattern_Count,
			};
			/**
			* 코드 에러 수정 기능을 결정합니다.
			*/
			enum EECCLevel
			{
				ECCLevel_L = 0b01, // 7%
				ECCLevel_M = 0b00, // 15%
				ECCLevel_Q = 0b11, // 25%
				ECCLevel_H = 0b10, // 30%
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
				EVersion_Count = 41,
				EVersion_None = 42,
			};
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refStrCode : 입력 문자코드 (Reference)
			* param[out] pImage : 출력 이미지 (Pointer)
			* param[in] pGeometry : 출력 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Write(_In_ const CString& refStrCode, _Out_ CRavidImage* pImage, CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refStrCode : 입력 문자코드 (Reference)
			* param[out] refImage : 출력 이미지 (Reference)
			* param[in] pGeometry : 출력 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Write(_In_ const CString& refStrCode, _Out_ CRavidImage& refImage, CRavidGeometry* pGeometry = nullptr);
			
			/**
			* QR 코드 표현 타입을 설정합니다.
			*
			* param[in] em : EModeIndicator enum QR 코드 표현 타입
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetModeIndicator(_In_ EModeIndicator em);
			
			/**
			* QR 코드 마스킹 패턴을 설정합니다.
			*
			* param[in] ep : EMaskingPattern enum QR 코드 표현 타입
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMaskingPattern(_In_ EMaskingPattern ep);
			
			/**
			* QR 코드 에러 수정 기능을 설정합니다.
			*
			* param[in] ee : EMaskingPattern enum QR 코드 표현 타입
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetECCLevel(_In_ EECCLevel ee);
			
			/**
			* QR 코드 버전을 설정합니다.
			*
			* param[in] ev : EVersion enum QR 코드 표현 타입
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetVersion(_In_ EVersion ev);
			
			/**
			* QR 코드 픽셀 크기를 설정합니다.
			*
			* param[in] nPixelSize : int 형으로 코드 점의 픽셀 크기
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetPixelSize(_In_ int nPixelSize);

		private:
			std::unique_ptr<CInternalQRCodeEncoder> m_pInternal;
		};
	}
}