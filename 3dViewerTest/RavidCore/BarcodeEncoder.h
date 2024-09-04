#pragma once

#include <memory>

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;

		class CInternalBarcodeEncoder;
		
		/**
		* 이미지로 1차원 바코드를 생성하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CBarcodeEncoder : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CBarcodeEncoder);

		public:
			/**
			* 바코드의 압축 방법입니다.
			*/
			enum E1DBarcodeEncoderType
			{
				E1DBarcodeEncoderType_EAN8,
				E1DBarcodeEncoderType_EAN13,
				E1DBarcodeEncoderType_EAN128,
				E1DBarcodeEncoderType_CODE11,
				E1DBarcodeEncoderType_CODE39,
				E1DBarcodeEncoderType_CODE128,
				E1DBarcodeEncoderType_MSI,
				E1DBarcodeEncoderType_UPCA,
				E1DBarcodeEncoderType_UPCE,
				E1DBarcodeEncoderType_Interleaved2Of5,
				E1DBarcodeEncoderType_Codabar
			};
			/**
			* 바코드의 압축 패턴입니다.
			*/
			enum E1DBarcodeEncoderPattern
			{
				E1DBarcodeEncoderType_LLLLLLRRRRRR,
				E1DBarcodeEncoderType_LLGLGGRRRRRR,
				E1DBarcodeEncoderType_LLGGLGRRRRRR,
				E1DBarcodeEncoderType_LLGGGLRRRRRR,
				E1DBarcodeEncoderType_LGLLGGRRRRRR,
				E1DBarcodeEncoderType_LGGLLGRRRRRR,
				E1DBarcodeEncoderType_LGGGLLRRRRRR,
				E1DBarcodeEncoderType_LGLGLGRRRRRR,
				E1DBarcodeEncoderType_LGLGGLRRRRRR,
				E1DBarcodeEncoderType_LGGLGLRRRRRR,
			};


			CBarcodeEncoder();
			virtual ~CBarcodeEncoder();
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refStrCode : 입력 문자열 (Reference)
			* param[out] pImage : 변환 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Write(_In_ const CString& refStrCode, _Out_ CRavidImage* pImage);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refStrCode : 입력 문자열 (Reference)
			* param[out] refImage : 변환 이미지 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Write(_In_ const CString& refStrCode, _Out_ CRavidImage& refImage);
			
			/**
			* 바코드 압축 방법을 설정합니다.
			*
			* param[in] eBET : 압축 방법 enum E1DBarcodeEncoderType
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetType(_In_ E1DBarcodeEncoderType eBET);
			
			/**
			* 바코드 압축 패턴을 설정합니다.
			*
			* param[in] eBEP : 압축 패턴 enum E1DBarcodeEncoderPattern
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetPattern(_In_ E1DBarcodeEncoderPattern eBEP);
			
			/**
			* 바코드 생성시 이미지 픽셀 크기를 설정합니다.
			*
			* param[in] nPixelSize : 2 이상의 값을 가집니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetPixelSize(_In_ int nPixelSize);
			
			/**
			* 현재 설정된 바코드 압축 방법을 가져옵니다.
			*
			* return value : enum E1DBarcodeEncoderType 의 타입을 반환합니다.
			*/
			E1DBarcodeEncoderType GetType(void);
			
			/**
			* 현재 설정된 바코드 압축 패턴을 가져옵니다.
			*
			* return value : enum E1DBarcodeEncoderPattern 의 타입을 반환합니다.
			*/
			E1DBarcodeEncoderPattern GetPattern(void);
			
			/**
			* 현재 설정된 바코드 생성시 이미지 픽셀 크기를 가져옵니다.
			*
			* return value : 기본값은 2로 설정된 값을 반환합니다.
			*/
			int GetPixelSize(void);

		private:
			std::unique_ptr<CInternalBarcodeEncoder> m_pInternal;
		};
	}
}


