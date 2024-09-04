#pragma once

#include <memory>

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;

		class CInternalDataMatrixEncoder;
		
		/**
		* 이미지로 2차원 데이터 메트릭스 코드를 생성하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CDataMatrixEncoder : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CDataMatrixEncoder);

		public:
			/**
			* 생성할 메트릭스 코드 타입입니다.
			*/
			enum EDataMatrixECCMode
			{
				EDataMatrixECCMode_200ASCII_AND_DIGIT = 0,
				EDataMatrixECCMode_200C40,
				EDataMatrixECCMode_200TEXT,
				EDataMatrixECCMode_200X12,
				EDataMatrixECCMode_200EDIFACT,
				EDataMatrixECCMode_200BASE256,
			};

			CDataMatrixEncoder();
			virtual ~CDataMatrixEncoder();
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refStrCode : 입력 문자코드 (Reference)
			* param[out] pImage : 출력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Write(_In_ const CString& refStrCode, _Out_ CRavidImage* pImage);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refStrCode : 입력 문자코드 (Reference)
			* param[out] refImage : 출력 이미지 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Write(_In_ const CString& refStrCode, _Out_ CRavidImage& refImage);
			
			/**
			* 메트릭스 코드 타입을 설정합니다.
			*
			* param[in] eDME : EDataMatrixECCMode enum 메트릭스 코드 타입
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMode(_In_ EDataMatrixECCMode eDME);
			
			/**
			* 메트릭스 코드의 픽셀 크기를 설정합니다.
			*
			* param[in] nPixelSize : int 형으로 코드 점의 픽셀 크기
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetPixelSize(_In_ int nPixelSize);
						
			/**
			* 설정된 메트릭스 코드 타입을 가져옵니다.
			*
			* return value : EDataMatrixECCMode enum 메트릭스 코드 타입을 반환합니다.
			*/
			EDataMatrixECCMode GetMode(void);
						
			/**
			* 설정된 메트릭스 코드의 픽셀 크기를 가져옵니다.
			*
			* return value : int 형으로 코드 점의 픽셀 크기를 반환합니다.
			*/
			int GetPixelSize(void);

		private:
			std::unique_ptr<CInternalDataMatrixEncoder> m_pInternal;

		};
	}
}


