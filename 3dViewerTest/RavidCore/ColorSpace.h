#pragma once

namespace Ravid
{
	namespace Algorithms
	{
		/**
		* 컬러 공간을 변환하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CColorSpace : public CRavidObject
		{
		public:
			CColorSpace();
			virtual ~CColorSpace();
			
			/**
			* RGB 데이터를 HSV (색조, 채도, 값)공간으로 변환합니다.
			*
			* param[in] wR : Red 데이터
			* param[in] wG : Green 데이터
			* param[in] wB : Blue 데이터
			* param[out] pH : Hue 색상 (Pointer)
			* param[out] pS : Saturation 채도 (Pointer)
			* param[out] pV : Value 값 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToHSV(_In_ WORD wR, _In_ WORD wG, _In_ WORD wB, _Out_ float* pH, _Out_ float* pS, _Out_ float* pV, _In_opt_ int nRGBDepth = 8);
			
			/**
			* RGB 데이터를 HSV (색조, 채도, 값)공간으로 변환합니다.
			*
			* param[in] wR : Red 데이터
			* param[in] wG : Green 데이터
			* param[in] wB : Blue 데이터
			* param[out] pRmcHSV : Hue 색상, Saturation 채도, Value 값 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToHSV(_In_ WORD wR, _In_ WORD wG, _In_ WORD wB, _Out_ Framework::CMultipleVariable* pRmcHSV, _In_opt_ int nRGBDepth = 8);
			
			/**
			* RGB 데이터를 HSV (색조, 채도, 값)공간으로 변환합니다.
			*
			* param[in] crRGB : RGB 데이터
			* param[out] pH : Hue 색상 (Pointer)
			* param[out] pS : Saturation 채도 (Pointer)
			* param[out] pV : Value 값 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToHSV(_In_ COLORREF crRGB, _Out_ float* pH, _Out_ float* pS, _Out_ float* pV);
			
			/**
			* RGB 데이터를 HSV (색조, 채도, 값)공간으로 변환합니다.
			*
			* param[in] crRGB : RGB 데이터
			* param[out] pRmcHSV : Hue 색상, Saturation 채도, Value 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToHSV(_In_ COLORREF crRGB, _Out_ Framework::CMultipleVariable* pRmcHSV);
			
			/**
			* RGB 데이터를 HSV (색조, 채도, 값)공간으로 변환합니다.
			*
			* param[in] rmvRGB : RGB 데이터 (Reference)
			* param[out] pH : Hue 색상 (Pointer)
			* param[out] pS : Saturation 채도 (Pointer)
			* param[out] pV : Value 값 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToHSV(_In_ Framework::CMultipleVariable& rmvRGB, _Out_ float* pH, _Out_ float* pS, _Out_ float* pV, _In_opt_ int nRGBDepth = 8);
			
			/**
			* RGB 데이터를 HSV (색조, 채도, 값)공간으로 변환합니다.
			*
			* param[in] rmvRGB : RGB 데이터 (Reference)
			* param[out] pRmcHSV : Hue 색상, Saturation 채도, Value 값 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToHSV(_In_ Framework::CMultipleVariable& rmvRGB, _Out_ Framework::CMultipleVariable* pRmcHSV, _In_opt_ int nRGBDepth = 8);
			
			/**
			* HSV (색조, 채도, 값)공간을 RGB로 변환합니다.
			*
			* param[in] fH : Hue 색상
			* param[in] fS : Saturation 채도
			* param[in] fV : Value 값
			* param[out] pR : Red 데이터 (Pointer)
			* param[out] pG : Green 데이터 (Pointer)
			* param[out] pB : Blue 데이터 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSVToRGB(_In_ float fH, _In_ float fS, _In_ float fV, _Out_ BYTE* pR, _Out_ BYTE* pG, _Out_ BYTE* pB);
			
			/**
			* HSV (색조, 채도, 값)공간을 RGB로 변환합니다.
			*
			* param[in] fH : Hue 색상
			* param[in] fS : Saturation 채도
			* param[in] fV : Value 값
			* param[out] pR : Red 데이터 (Pointer)
			* param[out] pG : Green 데이터 (Pointer)
			* param[out] pB : Blue 데이터 (Pointer)
			* param[in] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSVToRGB(_In_ float fH, _In_ float fS, _In_ float fV, _Out_ WORD* pR, _Out_ WORD* pG, _Out_ WORD* pB, _In_ int nRGBDepth);
			
			/**
			* HSV (색조, 채도, 값)공간을 RGB로 변환합니다.
			*
			* param[in] fH : Hue 색상
			* param[in] fS : Saturation 채도
			* param[in] fV : Value 값
			* param[out] pCrRGB : RGB 데이터 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSVToRGB(_In_ float fH, _In_ float fS, _In_ float fV, _Out_ COLORREF* pCrRGB);
			
			/**
			* HSV (색조, 채도, 값)공간을 RGB로 변환합니다.
			*
			* param[in] fH : Hue 색상
			* param[in] fS : Saturation 채도
			* param[in] fV : Value 값
			* param[out] pRmcRGB : RGB 데이터 (Pointer)
			* param[in] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSVToRGB(_In_ float fH, _In_ float fS, _In_ float fV, _Out_ Framework::CMultipleVariable* pRmcRGB, _In_ int nRGBDepth);
			
			/**
			* HSV (색조, 채도, 값)공간을 RGB로 변환합니다.
			*
			* param[in] rmvHSV : Hue 색상, Saturation 채도, Value 값 (Reference)
			* param[out] pR : Red 데이터 (Pointer)
			* param[out] pG : Green 데이터 (Pointer)
			* param[out] pB : Blue 데이터 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSVToRGB(_In_ Framework::CMultipleVariable& rmvHSV, _Out_ BYTE* pR, _Out_ BYTE* pG, _Out_ BYTE* pB);
			
			/**
			* HSV (색조, 채도, 값)공간을 RGB로 변환합니다.
			*
			* param[in] rmvHSV : Hue 색상, Saturation 채도, Value 값 (Reference)
			* param[out] pR : Red 데이터 (Pointer)
			* param[out] pG : Green 데이터 (Pointer)
			* param[out] pB : Blue 데이터 (Pointer)
			* param[in] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSVToRGB(_In_ Framework::CMultipleVariable& rmvHSV, _Out_ WORD* pR, _Out_ WORD* pG, _Out_ WORD* pB, _In_ int nRGBDepth);
			
			/**
			* HSV (색조, 채도, 값)공간을 RGB로 변환합니다.
			*
			* param[in] rmvHSV : Hue 색상, Saturation 채도, Value 값 (Reference)
			* param[out] pCrRGB : RGB 데이터 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSVToRGB(_In_ Framework::CMultipleVariable& rmvHSV, _Out_ COLORREF* pCrRGB);
			
			/**
			* HSV (색조, 채도, 값)공간을 RGB로 변환합니다.
			*
			* param[in] rmvHSV : Hue 색상, Saturation 채도, Value 값 (Reference)
			* param[out] pCrRGB : RGB 데이터 (Pointer)
			* param[in] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSVToRGB(_In_ Framework::CMultipleVariable& rmvHSV, _Out_ Framework::CMultipleVariable* pRmcRGB, _In_ int nRGBDepth);
			
			/**
			* RGB 데이터를 HSL (색조, 채도, 명도)공간으로 변환합니다.
			*
			* param[in] wR : Red 데이터
			* param[in] wG : Green 데이터
			* param[in] wB : Blue 데이터
			* param[out] pH : Hue 색상 (Pointer)
			* param[out] pS : Saturation 채도 (Pointer)
			* param[out] pL : Lightness 명도 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToHSL(_In_ WORD wR, _In_ WORD wG, _In_ WORD wB, _Out_ float* pH, _Out_ float* pS, _Out_ float* pL, _In_opt_ int nRGBDepth = 8);
			
			/**
			* RGB 데이터를 HSL (색조, 채도, 명도)공간으로 변환합니다.
			*
			* param[in] wR : Red 데이터
			* param[in] wG : Green 데이터
			* param[in] wB : Blue 데이터
			* param[out] pRmcHSL : Hue 색상, Saturation 채도, Lightness 명도 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToHSL(_In_ WORD wR, _In_ WORD wG, _In_ WORD wB, _Out_ Framework::CMultipleVariable* pRmcHSL, _In_opt_ int nRGBDepth = 8);
			
			/**
			* RGB 데이터를 HSL (색조, 채도, 명도)공간으로 변환합니다.
			*
			* param[in] crRGB : RGB 데이터
			* param[out] pH : Hue 색상 (Pointer)
			* param[out] pS : Saturation 채도 (Pointer)
			* param[out] pL : Lightness 명도 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToHSL(_In_ COLORREF crRGB, _Out_ float* pH, _Out_ float* pS, _Out_ float* pL);
			
			/**
			* RGB 데이터를 HSL (색조, 채도, 명도)공간으로 변환합니다.
			*
			* param[in] crRGB : RGB 데이터
			* param[out] pRmcHSL : Hue 색상, Saturation 채도, Lightness 명도 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToHSL(_In_ COLORREF crRGB, _Out_ Framework::CMultipleVariable* pRmcHSL);
			
			/**
			* RGB 데이터를 HSL (색조, 채도, 명도)공간으로 변환합니다.
			*
			* param[in] rmvRGB : RGB 데이터 (Reference)
			* param[out] pH : Hue 색상 (Pointer)
			* param[out] pS : Saturation 채도 (Pointer)
			* param[out] pL : Lightness 명도 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToHSL(_In_ Framework::CMultipleVariable& rmvRGB, _Out_ float* pH, _Out_ float* pS, _Out_ float* pL, _In_opt_ int nRGBDepth = 8);
			
			/**
			* RGB 데이터를 HSL (색조, 채도, 명도)공간으로 변환합니다.
			*
			* param[in] rmvRGB : RGB 데이터 (Reference)
			* param[out] pRmcHSL : Hue 색상, Saturation 채도, Lightness 명도 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToHSL(_In_ Framework::CMultipleVariable& rmvRGB, _Out_ Framework::CMultipleVariable* pRmcHSL, _In_opt_ int nRGBDepth = 8);
						
			/**
			* HSL (색조, 채도, 명도)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] fH : Hue 색상
			* param[in] fS : Saturation 채도
			* param[in] fL : Lightness 명도
			* param[out] pR : Red 데이터 (Pointer)
			* param[out] pG : Green 데이터 (Pointer)
			* param[out] pB : Blue 데이터 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSLToRGB(_In_ float fH, _In_ float fS, _In_ float fL, _Out_ BYTE* pR, _Out_ BYTE* pG, _Out_ BYTE* pB);
						
			/**
			* HSL (색조, 채도, 명도)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] fH : Hue 색상
			* param[in] fS : Saturation 채도
			* param[in] fL : Lightness 명도
			* param[out] pR : Red 데이터 (Pointer)
			* param[out] pG : Green 데이터 (Pointer)
			* param[out] pB : Blue 데이터 (Pointer)
			* param[in] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSLToRGB(_In_ float fH, _In_ float fS, _In_ float fL, _Out_ WORD* pR, _Out_ WORD* pG, _Out_ WORD* pB, _In_ int nRGBDepth);
						
			/**
			* HSL (색조, 채도, 명도)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] fH : Hue 색상
			* param[in] fS : Saturation 채도
			* param[in] fL : Lightness 명도
			* param[out] pCrRGB : RGB 데이터 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSLToRGB(_In_ float fH, _In_ float fS, _In_ float fL, _Out_ COLORREF* pCrRGB);
						
			/**
			* HSL (색조, 채도, 명도)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] fH : Hue 색상
			* param[in] fS : Saturation 채도
			* param[in] fL : Lightness 명도
			* param[out] pRmcRGB : RGB 데이터 (Pointer)
			* param[in] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSLToRGB(_In_ float fH, _In_ float fS, _In_ float fL, _Out_ Framework::CMultipleVariable* pRmcRGB, _In_ int nRGBDepth);
						
			/**
			* HSL (색조, 채도, 명도)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] rmvHSL : Hue 색상, Saturation 채도, Lightness 명도 (Reference)
			* param[out] pR : Red 데이터 (Pointer)
			* param[out] pG : Green 데이터 (Pointer)
			* param[out] pB : Blue 데이터 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSLToRGB(_In_ Framework::CMultipleVariable& rmvHSL, _Out_ BYTE* pR, _Out_ BYTE* pG, _Out_ BYTE* pB);
						
			/**
			* HSL (색조, 채도, 명도)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] rmvHSL : Hue 색상, Saturation 채도, Lightness 명도 (Reference)
			* param[out] pR : Red 데이터 (Pointer)
			* param[out] pG : Green 데이터 (Pointer)
			* param[out] pB : Blue 데이터 (Pointer)
			* param[in] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSLToRGB(_In_ Framework::CMultipleVariable& rmvHSL, _Out_ WORD* pR, _Out_ WORD* pG, _Out_ WORD* pB, _In_ int nRGBDepth);
						
			/**
			* HSL (색조, 채도, 명도)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] rmvHSL : Hue 색상, Saturation 채도, Lightness 명도 (Reference)
			* param[out] pCrRGB : RGB 데이터 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSLToRGB(_In_ Framework::CMultipleVariable& rmvHSL, _Out_ COLORREF* pCrRGB);
						
			/**
			* HSL (색조, 채도, 명도)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] rmvHSL : Hue 색상, Saturation 채도, Lightness 명도 (Reference)
			* param[out] pRmcRGB : RGB 데이터 (Pointer)
			* param[in] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertHSLToRGB(_In_ Framework::CMultipleVariable& rmvHSL, _Out_ Framework::CMultipleVariable* pRmcRGB, _In_ int nRGBDepth);
			
			/**
			* RGB 데이터를 CMYK (시안, 마젠타, 노랑, 검정)공간으로 변환합니다.
			*
			* param[in] wR : Red 데이터
			* param[in] wG : Green 데이터
			* param[in] wB : Blue 데이터
			* param[out] pC : Cyan 시안 (Pointer)
			* param[out] pM : Magenta 마젠타 (Pointer)
			* param[out] pY : Yellow 노랑 (Pointer)
			* param[out] pK : Key(Black) 검정 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToCMYK(_In_ WORD wR, _In_ WORD wG, _In_ WORD wB, _Out_ WORD* pC, _Out_ WORD* pM, _Out_ WORD* pY, _Out_ WORD* pK, _In_opt_ int nRGBDepth = 8);
			
			/**
			* RGB 데이터를 CMYK (시안, 마젠타, 노랑, 검정)공간으로 변환합니다.
			*
			* param[in] wR : Red 데이터
			* param[in] wG : Green 데이터
			* param[in] wB : Blue 데이터
			* param[out] pRmcCMYK : Cyan 시안, Magenta 마젠타, Yellow 노랑, Key(Black) 검정 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToCMYK(_In_ WORD wR, _In_ WORD wG, _In_ WORD wB, _Out_ Framework::CMultipleVariable* pRmcCMYK, _In_opt_ int nRGBDepth = 8);
			
			/**
			* RGB 데이터를 CMYK (시안, 마젠타, 노랑, 검정)공간으로 변환합니다.
			*
			* param[in] crRGB : RGB 데이터
			* param[out] pC : Cyan 시안 (Pointer)
			* param[out] pM : Magenta 마젠타 (Pointer)
			* param[out] pY : Yellow 노랑 (Pointer)
			* param[out] pK : Key(Black) 검정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToCMYK(_In_ COLORREF crRGB, _Out_ WORD* pC, _Out_ WORD* pM, _Out_ WORD* pY, _Out_ WORD* pK);
			
			/**
			* RGB 데이터를 CMYK (시안, 마젠타, 노랑, 검정)공간으로 변환합니다.
			*
			* param[in] crRGB : RGB 데이터
			* param[out] pRmcCMYK : Cyan 시안, Magenta 마젠타, Yellow 노랑, Key(Black) 검정 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToCMYK(_In_ COLORREF crRGB, _Out_ Framework::CMultipleVariable* pRmcCMYK);
			
			/**
			* RGB 데이터를 CMYK (시안, 마젠타, 노랑, 검정)공간으로 변환합니다.
			*
			* param[in] rmvRGB : RGB 데이터 (Reference)
			* param[out] pC : Cyan 시안 (Pointer)
			* param[out] pM : Magenta 마젠타 (Pointer)
			* param[out] pY : Yellow 노랑 (Pointer)
			* param[out] pK : Key(Black) 검정 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToCMYK(_In_ Framework::CMultipleVariable& rmvRGB, _Out_ WORD* pC, _Out_ WORD* pM, _Out_ WORD* pY, _Out_ WORD* pK, _In_opt_ int nRGBDepth = 8);
			
			/**
			* RGB 데이터를 CMYK (시안, 마젠타, 노랑, 검정)공간으로 변환합니다.
			*
			* param[in] rmvRGB : RGB 데이터 (Reference)
			* param[out] pRmcCMYK : Cyan 시안, Magenta 마젠타, Yellow 노랑, Key(Black) 검정 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToCMYK(_In_ Framework::CMultipleVariable& rmvRGB, _Out_ Framework::CMultipleVariable* pRmcCMYK, _In_opt_ int nRGBDepth = 8);
			
			/**
			* CMYK (시안, 마젠타, 노랑, 검정)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] wC : Cyan 시안
			* param[in] wM : Magenta 마젠타
			* param[in] wY : Yellow 노랑
			* param[in] wK : Key(Black) 검정
			* param[out] pR : Red 데이터 (Pointer)
			* param[out] pG : Green 데이터 (Pointer)
			* param[out] pB : Blue 데이터 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCMYKToRGB(_In_ WORD wC, _In_ WORD wM, _In_ WORD wY, _In_ WORD wK, _Out_ BYTE* pR, _Out_ BYTE* pG, _Out_ BYTE* pB);
			
			/**
			* CMYK (시안, 마젠타, 노랑, 검정)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] wC : Cyan 시안
			* param[in] wM : Magenta 마젠타
			* param[in] wY : Yellow 노랑
			* param[in] wK : Key(Black) 검정
			* param[out] pR : Red 데이터 (Pointer)
			* param[out] pG : Green 데이터 (Pointer)
			* param[out] pB : Blue 데이터 (Pointer)
			* param[in] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCMYKToRGB(_In_ WORD wC, _In_ WORD wM, _In_ WORD wY, _In_ WORD wK, _Out_ WORD* pR, _Out_ WORD* pG, _Out_ WORD* pB, _In_ int nRGBDepth);
			
			/**
			* CMYK (시안, 마젠타, 노랑, 검정)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] wC : Cyan 시안
			* param[in] wM : Magenta 마젠타
			* param[in] wY : Yellow 노랑
			* param[in] wK : Key(Black) 검정
			* param[out] pCrRGB : RGB 데이터 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCMYKToRGB(_In_ WORD wC, _In_ WORD wM, _In_ WORD wY, _In_ WORD wK, _Out_ COLORREF* pCrRGB);
			
			/**
			* CMYK (시안, 마젠타, 노랑, 검정)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] wC : Cyan 시안
			* param[in] wM : Magenta 마젠타
			* param[in] wY : Yellow 노랑
			* param[in] wK : Key(Black) 검정
			* param[out] pRmcRGB : RGB 데이터 (Pointer)
			* param[in_opt] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCMYKToRGB(_In_ WORD wC, _In_ WORD wM, _In_ WORD wY, _In_ WORD wK, _Out_ Framework::CMultipleVariable* pRmcRGB, _In_ int nRGBDepth);
			
			/**
			* CMYK (시안, 마젠타, 노랑, 검정)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] rmvCMYK : Cyan 시안, Magenta 마젠타, Yellow 노랑, Key(Black) 검정 (Reference)
			* param[out] pR : Red 데이터 (Pointer)
			* param[out] pG : Green 데이터 (Pointer)
			* param[out] pB : Blue 데이터 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCMYKToRGB(_In_ Framework::CMultipleVariable& rmvCMYK, _Out_ BYTE* pR, _Out_ BYTE* pG, _Out_ BYTE* pB);
			
			/**
			* CMYK (시안, 마젠타, 노랑, 검정)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] rmvCMYK : Cyan 시안, Magenta 마젠타, Yellow 노랑, Key(Black) 검정 (Reference)
			* param[out] pRmcRGB : RGB 데이터 (Pointer)
			* param[out] pR : Red 데이터 (Pointer)
			* param[out] pG : Green 데이터 (Pointer)
			* param[out] pB : Blue 데이터 (Pointer)
			* param[in] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCMYKToRGB(_In_ Framework::CMultipleVariable& rmvCMYK, _Out_ WORD* pR, _Out_ WORD* pG, _Out_ WORD* pB, _In_ int nRGBDepth);
			
			/**
			* CMYK (시안, 마젠타, 노랑, 검정)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] rmvCMYK : Cyan 시안, Magenta 마젠타, Yellow 노랑, Key(Black) 검정 (Reference)
			* param[out] pCrRGB : RGB 데이터 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCMYKToRGB(_In_ Framework::CMultipleVariable& rmvCMYK, _Out_ COLORREF* pCrRGB);
			
			/**
			* CMYK (시안, 마젠타, 노랑, 검정)공간을 RGB 데이터로 변환합니다.
			*
			* param[in] rmvCMYK : Cyan 시안, Magenta 마젠타, Yellow 노랑, Key(Black) 검정 (Reference)
			* param[out] pCrRGB : RGB 데이터 (Pointer)
			* param[in] nRGBDepth : RGB Depth
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertCMYKToRGB(_In_ Framework::CMultipleVariable& rmvCMYK, _Out_ Framework::CMultipleVariable* pRmcRGB, _In_ int nRGBDepth);

		};
	}
}

