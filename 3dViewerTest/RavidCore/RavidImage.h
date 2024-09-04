#pragma once

#include "MultipleVariable.h"
#include "MultipleCondition.h"

#include <vector>
#include <complex>

class CRavidGeometry;

namespace Ravid
{
	namespace Mathematics
	{
		class CMatrix;
	}
	
	namespace Algorithms
	{
		class CImageOperationInfo;

		class CInternalRavidImage;

		class AFX_EXT_CLASS CRavidImage : public CRavidObject
		{
			RavidUseDynamicCreation();

		public:
			/**
			* 이미지 보간 처리 방법을 설정합니다.
			*/
			enum EInterpolationMethod
			{
				EInterpolationMethod_NearestNeighbor = 0,	// 근처값
				EInterpolationMethod_Bilinear,				// 2차원 보간
				EInterpolationMethod_Bicubic,				// 3차 스플라인 보간
			};
			/**
			* 헤리스코너 방법을 설정합니다.
			*/
			enum EHarrisCornerDetectorThresholdType
			{
				EHarrisCornerDetectorThresholdType_Absolute = 0,	// 절대값
				EHarrisCornerDetectorThresholdType_Relative,		// 적응값
			};
			/**
			* 케니 엣지 방법을 설정합니다.
			*/
			enum ECannyEdgeDetectorThresholdType
			{
				ECannyEdgeDetectorThresholdType_Absolute = 0,	// 절대값
				ECannyEdgeDetectorThresholdType_Relative,		// 적응값
			};
			/**
			* 프로젝션 처리 방법을 설정합니다.
			*/
			enum EProjectionDirectionType
			{
				EProjectionDirectionType_Row = 0,	// 행
				EProjectionDirectionType_Column,	// 열
				EProjectionDirectionType_Angle		// 각도
			};
			/**
			* FFT 처리 방법을 설정합니다.
			*/
			enum EFFTMethod
			{
				EFFTMethod_ForwardFloatNone = 0,	// Float 32 일반
				EFFTMethod_ForwardFloatShift,		// Float 32 쉬프트
				EFFTMethod_ForwardDoubleNone,		// Float 64 일반
				EFFTMethod_ForwardDoubleShift,		// Float 64 쉬프트
				EFFTMethod_Backward,				// 복원
			};
			/**
			* 모폴로지 연산 방법을 설정합니다.
			*/
			enum EMorphologyMethod
			{
				EMorphologyMethod_Erode = 0,		// 침식
				EMorphologyMethod_Dilate,			// 확장
				EMorphologyMethod_Open,				// 열기
				EMorphologyMethod_Close,			// 닫기
				EMorphologyMethod_Gradient,			// 경계
				EMorphologyMethod_WhiteTopHat,		// 밝은 영역 대비 어두운 영역 편차
				EMorphologyMethod_BlackTopHat,		// 어두운 영역 대비 밝은 영역 편차
				EMorphologyMethod_Median3x3,		// 중앙값 3 by 3
				EMorphologyMethod_Median5x5,		// 중앙값 5 by 5
				EMorphologyMethod_Median7x7,		// 중앙값 7 by 7
				EMorphologyMethod_MedianMxN,		// 중앙값 N by N
			};
			/**
			* 적응적 이진화 처리 시 반전을 설정합니다.
			*/
			enum EAdaptiveThresholdType
			{
				EAdaptiveThresholdType_Binary = 0,	// 정상
				EAdaptiveThresholdType_Binary_Inv,	// 반전
			};
			/**
			* 적응적 이진화 처리 시 필터를 설정합니다.
			*/
			enum EAdaptiveThresholdMethod
			{
				EAdaptiveThresholdMethod_Mean = 0,	//평균
				EAdaptiveThresholdMethod_Gaussian,	//가우시안
			};
			/**
			* 이진화 처리 시 다중 채널의 논리연산 처리 방법을 설정합니다.
			*/
			enum EThresholdLogicalOperator
			{
				EThresholdLogicalOperator_None = 0,	// 개별 처리
				EThresholdLogicalOperator_And,		// 모두 만족시 모두 적용
				EThresholdLogicalOperator_Or,		// 하나라도 만족시 모두 적용

			};
			/**
			* 이미지 픽셀 표현 크기 및 표현 방법 변경을 설정합니다.
			*/
			enum EConvertDepth
			{
				EConvertDepth_ToU8 = 0x00008,		// 부호가 없는 UNSIGNED 8 BIT 로 변경
				EConvertDepth_ToU10 = 0x0000A,		// 부호가 없는 UNSIGNED 10 BIT 로 변경
				EConvertDepth_ToU12 = 0x0000C,		// 부호가 없는 UNSIGNED 12 BIT 로 변경
				EConvertDepth_ToU13 = 0x0000D,		// 부호가 없는 UNSIGNED 13 BIT 로 변경
				EConvertDepth_ToU14 = 0x0000E,		// 부호가 없는 UNSIGNED 14 BIT 로 변경
				EConvertDepth_ToU16 = 0x00010,		// 부호가 없는 UNSIGNED 16 BIT 로 변경
				EConvertDepth_ToU32 = 0x00020,		// 부호가 없는 UNSIGNED 32 BIT 로 변경
				EConvertDepth_ToU64 = 0x00040,		// 부호가 없는 UNSIGNED 64 BIT 로 변경

				EConvertDepth_ToS8 = 0x00108,		// 부호가 있는 SIGNED 8 BIT 로 변경
				EConvertDepth_ToS10 = 0x0010A,		// 부호가 있는 SIGNED 10 BIT 로 변경
				EConvertDepth_ToS12 = 0x0010C,		// 부호가 있는 SIGNED 12 BIT 로 변경
				EConvertDepth_ToS13 = 0x0010D,		// 부호가 있는 SIGNED 13 BIT 로 변경
				EConvertDepth_ToS14 = 0x0010E,		// 부호가 있는 SIGNED 14 BIT 로 변경
				EConvertDepth_ToS16 = 0x00110,		// 부호가 있는 SIGNED 16 BIT 로 변경
				EConvertDepth_ToS32 = 0x00120,		// 부호가 있는 SIGNED 32 BIT 로 변경
				EConvertDepth_ToS64 = 0x00140,		// 부호가 있는 SIGNED 64 BIT 로 변경

				EConvertDepth_ToF32 = 0x00220,		// 부호가 있는 FLOATING 32 BIT 로 변경
				EConvertDepth_ToF64 = 0x00240,		// 부호가 있는 FLOATING 64 BIT 로 변경
			};
			/**
			* 이미지 픽셀 크기 변경 시 변경 크기에 맞게 값을 변경 혹은 유지 하도록 설정합니다.
			*/
			enum EConvertDepthValueMode
			{
				EConvertDepthValueMode_ChangeValue = 0,	// 목표 픽셀 크기에 맞게 변경
				EConvertDepthValueMode_KeepValue,		// 현재 값을 유지
			};
			/**
			* 이미지의 방향의 반전을 설정합니다.
			*/
			enum EFlipType
			{
				EFlipType_X = 0,	// 가로 방향 반전
				EFlipType_Y,		// 세로 방향 반전
				EFlipType_XY,		// 가로, 세로 방향 반전
			};
			/**
			* 이미지 컨볼루션 시 미리 정의된 프리셋을 사용합니다.
			*/
			enum EConvolutionPreset
			{
				EConvolutionPreset_Uniform3x3 = 0,	// 유니폼 필터 3 by 3
				EConvolutionPreset_Gaussian3x3,		// 가우시안 필터 3 by 3
				EConvolutionPreset_LowPass1,		// 저주파 통과 필터1 3 by 3
				EConvolutionPreset_LowPass2,		// 저주파 통과 필터2 3 by 3
				EConvolutionPreset_LowPass3,		// 저주파 통과 필터3 3 by 3

				EConvolutionPreset_Uniform5x5,		// 유니폼 필터 5 by 5
				EConvolutionPreset_Gaussian5x5,		// 가우시안 필터 5 by 5

				EConvolutionPreset_Uniform7x7,		// 유니폼 필터 7 by 7
				EConvolutionPreset_Gaussian7x7,		// 가우시안 필터 7 by 7

				EConvolutionPreset_Sharpen,			// 샤프 필터 3 by 3
				EConvolutionPreset_HighPass1,		// 고주파 통과 필터1 3 by 3
				EConvolutionPreset_HighPass2,		// 고주파 통과 필터2 3 by 3

				EConvolutionPreset_GradientX,		// 기울기 x 방향 필터 3 by 3
				EConvolutionPreset_GradientY,		// 기울기 y 방향 필터 3 by 3
				EConvolutionPreset_Gradient,		// 기울기 xy 방향 필터 3 by 3

				EConvolutionPreset_SobelX,			// 엣지 x 방향 필터 3 by 3
				EConvolutionPreset_SobelY,			// 엣지 y 방향 필터 3 by 3
				EConvolutionPreset_Sobel,			// 엣지 xy 방향 필터 3 by 3

				EConvolutionPreset_PrewittX,		// 엣지 x 방향 필터 3 by 3
				EConvolutionPreset_PrewittY,		// 엣지 y 방향 필터 3 by 3
				EConvolutionPreset_Prewitt,			// 엣지 xy 방향 필터 3 by 3

				EConvolutionPreset_LaplacianX,		// 기울기 x 방향 수렴 필터 3 by 3
				EConvolutionPreset_LaplacianY,		// 기울기 Y 방향 수렴 필터 3 by 3
				EConvolutionPreset_Laplacian4,		// 기울기 xy 4 방향 수렴 필터 3 by 3
				EConvolutionPreset_Laplacian8,		// 기울기 xy 8 방향 수렴 필터 3 by 3

				EConvolutionPreset_CompassN,		// 기울기 아래->위 방향 필터 3 by 3
				EConvolutionPreset_CompassNW,		// 기울기 아래->위, 오른쪽->왼쪽 방향 필터 3 by 3
				EConvolutionPreset_CompassW,		// 기울기 오른쪽->왼쪽 방향 필터 3 by 3
				EConvolutionPreset_CompassSW,		// 기울기 위->아래, 오른쪽->왼쪽 방향 필터 3 by 3
				EConvolutionPreset_CompassS,		// 기울기 위->아래 방향 필터 3 by 3
				EConvolutionPreset_CompassSE,		// 기울기 위->아래, 왼쪽->오른쪽 방향 필터 3 by 3
				EConvolutionPreset_CompassE,		// 기울기 왼쪽->오른쪽 방향 필터 3 by 3
				EConvolutionPreset_CompassNE,		// 기울기 아래->위, 왼쪽->오른쪽 방향 필터 3 by 3
			};
			/**
			* 이미지 데이터의 타입을 사용합니다.
			*/
			enum EValueType
			{
				EValueType_Unsigned = 0,	// 부호 미포함 
				EValueType_Signed,			// 부호 포함
				EValueType_FloatingPoint	// 소수점
			};
			/**
			* 이미지 데이터의 채널 및 타입, 비트를 표현합니다.
			*/
			enum EValueFormat
			{
				EValueFormat_1C_U8 = 0x00008,	// 채널 1 : 부호 미포함 : 8 비트
				EValueFormat_1C_U10 = 0x0000A,	// 채널 1 : 부호 미포함 : 10 비트
				EValueFormat_1C_U12 = 0x0000C,	// 채널 1 : 부호 미포함 : 12 비트
				EValueFormat_1C_U13 = 0x0000D,	// 채널 1 : 부호 미포함 : 13 비트
				EValueFormat_1C_U14 = 0x0000E,	// 채널 1 : 부호 미포함 : 14 비트
				EValueFormat_1C_U16 = 0x00010,	// 채널 1 : 부호 미포함 : 16 비트
				EValueFormat_1C_U32 = 0x00020,	// 채널 1 : 부호 미포함 : 32 비트
				EValueFormat_1C_U64 = 0x00040,	// 채널 1 : 부호 미포함 : 64 비트

				EValueFormat_1C_S8 = 0x00108,	// 채널 1 : 부호 포함 : 8 비트
				EValueFormat_1C_S10 = 0x0010A,	// 채널 1 : 부호 포함 : 10 비트
				EValueFormat_1C_S12 = 0x0010C,	// 채널 1 : 부호 포함 : 12 비트
				EValueFormat_1C_S13 = 0x0010D,	// 채널 1 : 부호 포함 : 13 비트
				EValueFormat_1C_S14 = 0x0010E,	// 채널 1 : 부호 포함 : 14 비트
				EValueFormat_1C_S16 = 0x00110,	// 채널 1 : 부호 포함 : 16 비트
				EValueFormat_1C_S32 = 0x00120,	// 채널 1 : 부호 포함 : 32 비트
				EValueFormat_1C_S64 = 0x00140,	// 채널 1 : 부호 포함 : 64 비트

				EValueFormat_1C_F32 = 0x00220,	// 채널 1 : 소수점 : 32 비트
				EValueFormat_1C_F64 = 0x00240,	// 채널 1 : 소수점 : 64 비트
				
				EValueFormat_2C_U8 = 0x01008,	// 채널 2 : 부호 미포함 : 8 비트
				EValueFormat_2C_U10 = 0x0100A,	// 채널 2 : 부호 미포함 : 10 비트
				EValueFormat_2C_U12 = 0x0100C,	// 채널 2 : 부호 미포함 : 12 비트
				EValueFormat_2C_U13 = 0x0100D,	// 채널 2 : 부호 미포함 : 13 비트
				EValueFormat_2C_U14 = 0x0100E,	// 채널 2 : 부호 미포함 : 14 비트
				EValueFormat_2C_U16 = 0x01010,	// 채널 2 : 부호 미포함 : 16 비트
				EValueFormat_2C_U32 = 0x01020,	// 채널 2 : 부호 미포함 : 32 비트
				EValueFormat_2C_U64 = 0x01040,	// 채널 2 : 부호 미포함 : 64 비트

				EValueFormat_2C_S8 = 0x01108,	// 채널 2 : 부호 포함 : 8 비트
				EValueFormat_2C_S10 = 0x0110A,	// 채널 2 : 부호 포함 : 10 비트
				EValueFormat_2C_S12 = 0x0110C,	// 채널 2 : 부호 포함 : 12 비트
				EValueFormat_2C_S13 = 0x0110D,	// 채널 2 : 부호 포함 : 13 비트
				EValueFormat_2C_S14 = 0x0110E,	// 채널 2 : 부호 포함 : 14 비트
				EValueFormat_2C_S16 = 0x01110,	// 채널 2 : 부호 포함 : 16 비트
				EValueFormat_2C_S32 = 0x01120,	// 채널 2 : 부호 포함 : 32 비트
				EValueFormat_2C_S64 = 0x01140,	// 채널 2 : 부호 포함 : 64 비트

				EValueFormat_2C_F32 = 0x01220,	// 채널 2 : 소수점 : 32 비트
				EValueFormat_2C_F64 = 0x01240,	// 채널 2 : 소수점 : 64 비트
				
				EValueFormat_3C_U8 = 0x02008,	// 채널 3 : 부호 미포함 : 8 비트
				EValueFormat_3C_U10 = 0x0200A,	// 채널 3 : 부호 미포함 : 10 비트
				EValueFormat_3C_U12 = 0x0200C,	// 채널 3 : 부호 미포함 : 12 비트
				EValueFormat_3C_U13 = 0x0200D,	// 채널 3 : 부호 미포함 : 13 비트
				EValueFormat_3C_U14 = 0x0200E,	// 채널 3 : 부호 미포함 : 14 비트
				EValueFormat_3C_U16 = 0x02010,	// 채널 3 : 부호 미포함 : 16 비트
				EValueFormat_3C_U32 = 0x02020,	// 채널 3 : 부호 미포함 : 32 비트
				EValueFormat_3C_U64 = 0x02040,	// 채널 3 : 부호 미포함 : 64 비트

				EValueFormat_3C_S8 = 0x02108,	// 채널 3 : 부호 포함 : 8 비트
				EValueFormat_3C_S10 = 0x0210A,	// 채널 3 : 부호 포함 : 10 비트
				EValueFormat_3C_S12 = 0x0210C,	// 채널 3 : 부호 포함 : 12 비트
				EValueFormat_3C_S13 = 0x0210D,	// 채널 3 : 부호 포함 : 13 비트
				EValueFormat_3C_S14 = 0x0210E,	// 채널 3 : 부호 포함 : 14 비트
				EValueFormat_3C_S16 = 0x02110,	// 채널 3 : 부호 포함 : 16 비트
				EValueFormat_3C_S32 = 0x02120,	// 채널 3 : 부호 포함 : 32 비트
				EValueFormat_3C_S64 = 0x02140,	// 채널 3 : 부호 포함 : 64 비트

				EValueFormat_3C_F32 = 0x02220,	// 채널 3 : 소수점 : 32 비트
				EValueFormat_3C_F64 = 0x02240,	// 채널 3 : 소수점 : 64 비트
				
				EValueFormat_4C_U8 = 0x03008,	// 채널 4 : 부호 미포함 : 8 비트
				EValueFormat_4C_U10 = 0x0300A, 	// 채널 4 : 부호 미포함 : 10 비트
				EValueFormat_4C_U12 = 0x0300C, 	// 채널 4 : 부호 미포함 : 12 비트
				EValueFormat_4C_U13 = 0x0300D, 	// 채널 4 : 부호 미포함 : 13 비트
				EValueFormat_4C_U14 = 0x0300E, 	// 채널 4 : 부호 미포함 : 14 비트
				EValueFormat_4C_U16 = 0x03010, 	// 채널 4 : 부호 미포함 : 16 비트
				EValueFormat_4C_U32 = 0x03020, 	// 채널 4 : 부호 미포함 : 32 비트
				EValueFormat_4C_U64 = 0x03040, 	// 채널 4 : 부호 미포함 : 64 비트

				EValueFormat_4C_S8 = 0x03108,  	// 채널 4 : 부호 포함 : 8 비트
				EValueFormat_4C_S10 = 0x0310A, 	// 채널 4 : 부호 포함 : 10 비트
				EValueFormat_4C_S12 = 0x0310C, 	// 채널 4 : 부호 포함 : 12 비트
				EValueFormat_4C_S13 = 0x0310D, 	// 채널 4 : 부호 포함 : 13 비트
				EValueFormat_4C_S14 = 0x0310E, 	// 채널 4 : 부호 포함 : 14 비트
				EValueFormat_4C_S16 = 0x03110, 	// 채널 4 : 부호 포함 : 16 비트
				EValueFormat_4C_S32 = 0x03120, 	// 채널 4 : 부호 포함 : 32 비트
				EValueFormat_4C_S64 = 0x03140, 	// 채널 4 : 부호 포함 : 64 비트

				EValueFormat_4C_F32 = 0x03220,	// 채널 4 : 소수점 : 32 비트
				EValueFormat_4C_F64 = 0x03240,	// 채널 4 : 소수점 : 64 비트
				
				EValueFormat_5C_U8 = 0x04008,  	// 채널 5 : 부호 미포함 : 8 비트
				EValueFormat_5C_U10 = 0x0400A, 	// 채널 5 : 부호 미포함 : 10 비트
				EValueFormat_5C_U12 = 0x0400C, 	// 채널 5 : 부호 미포함 : 12 비트
				EValueFormat_5C_U13 = 0x0400D, 	// 채널 5 : 부호 미포함 : 13 비트
				EValueFormat_5C_U14 = 0x0400E, 	// 채널 5 : 부호 미포함 : 14 비트
				EValueFormat_5C_U16 = 0x04010, 	// 채널 5 : 부호 미포함 : 16 비트
				EValueFormat_5C_U32 = 0x04020, 	// 채널 5 : 부호 미포함 : 32 비트
				EValueFormat_5C_U64 = 0x04040, 	// 채널 5 : 부호 미포함 : 64 비트

				EValueFormat_5C_S8 = 0x04108,  	// 채널 5 : 부호 포함 : 8 비트
				EValueFormat_5C_S10 = 0x0410A, 	// 채널 5 : 부호 포함 : 10 비트
				EValueFormat_5C_S12 = 0x0410C, 	// 채널 5 : 부호 포함 : 12 비트
				EValueFormat_5C_S13 = 0x0410D, 	// 채널 5 : 부호 포함 : 13 비트
				EValueFormat_5C_S14 = 0x0410E, 	// 채널 5 : 부호 포함 : 14 비트
				EValueFormat_5C_S16 = 0x04110, 	// 채널 5 : 부호 포함 : 16 비트
				EValueFormat_5C_S32 = 0x04120, 	// 채널 5 : 부호 포함 : 32 비트
				EValueFormat_5C_S64 = 0x04140, 	// 채널 5 : 부호 포함 : 64 비트

				EValueFormat_5C_F32 = 0x04220,	// 채널 5 : 소수점 : 32 비트
				EValueFormat_5C_F64 = 0x04240,	// 채널 5 : 소수점 : 64 비트
					
				EValueFormat_6C_U8 = 0x05008,  	// 채널 6 : 부호 미포함 : 8 비트
				EValueFormat_6C_U10 = 0x0500A, 	// 채널 6 : 부호 미포함 : 10 비트
				EValueFormat_6C_U12 = 0x0500C, 	// 채널 6 : 부호 미포함 : 12 비트
				EValueFormat_6C_U13 = 0x0500D, 	// 채널 6 : 부호 미포함 : 13 비트
				EValueFormat_6C_U14 = 0x0500E, 	// 채널 6 : 부호 미포함 : 14 비트
				EValueFormat_6C_U16 = 0x05010, 	// 채널 6 : 부호 미포함 : 16 비트
				EValueFormat_6C_U32 = 0x05020, 	// 채널 6 : 부호 미포함 : 32 비트
				EValueFormat_6C_U64 = 0x05040, 	// 채널 6 : 부호 미포함 : 64 비트

				EValueFormat_6C_S8 = 0x05108, 	// 채널 6 : 부호 포함 : 8 비트
				EValueFormat_6C_S10 = 0x0510A,	// 채널 6 : 부호 포함 : 10 비트
				EValueFormat_6C_S12 = 0x0510C,	// 채널 6 : 부호 포함 : 12 비트
				EValueFormat_6C_S13 = 0x0510D,	// 채널 6 : 부호 포함 : 13 비트
				EValueFormat_6C_S14 = 0x0510E,	// 채널 6 : 부호 포함 : 14 비트
				EValueFormat_6C_S16 = 0x05110,	// 채널 6 : 부호 포함 : 16 비트
				EValueFormat_6C_S32 = 0x05120,	// 채널 6 : 부호 포함 : 32 비트
				EValueFormat_6C_S64 = 0x05140,	// 채널 6 : 부호 포함 : 64 비트

				EValueFormat_6C_F32 = 0x05220,	// 채널 6 : 소수점 : 32 비트
				EValueFormat_6C_F64 = 0x05240,	// 채널 6 : 소수점 : 64 비트
					
				EValueFormat_7C_U8 = 0x06008, 	// 채널 7 : 부호 미포함 : 8 비트
				EValueFormat_7C_U10 = 0x0600A,	// 채널 7 : 부호 미포함 : 10 비트
				EValueFormat_7C_U12 = 0x0600C,	// 채널 7 : 부호 미포함 : 12 비트
				EValueFormat_7C_U13 = 0x0600D,	// 채널 7 : 부호 미포함 : 13 비트
				EValueFormat_7C_U14 = 0x0600E,	// 채널 7 : 부호 미포함 : 14 비트
				EValueFormat_7C_U16 = 0x06010,	// 채널 7 : 부호 미포함 : 16 비트
				EValueFormat_7C_U32 = 0x06020,	// 채널 7 : 부호 미포함 : 32 비트
				EValueFormat_7C_U64 = 0x06040,	// 채널 7 : 부호 미포함 : 64 비트

				EValueFormat_7C_S8 = 0x06108,  	// 채널 7 : 부호 포함 : 8 비트
				EValueFormat_7C_S10 = 0x0610A, 	// 채널 7 : 부호 포함 : 10 비트
				EValueFormat_7C_S12 = 0x0610C, 	// 채널 7 : 부호 포함 : 12 비트
				EValueFormat_7C_S13 = 0x0610D, 	// 채널 7 : 부호 포함 : 13 비트
				EValueFormat_7C_S14 = 0x0610E, 	// 채널 7 : 부호 포함 : 14 비트
				EValueFormat_7C_S16 = 0x06110, 	// 채널 7 : 부호 포함 : 16 비트
				EValueFormat_7C_S32 = 0x06120, 	// 채널 7 : 부호 포함 : 32 비트
				EValueFormat_7C_S64 = 0x06140, 	// 채널 7 : 부호 포함 : 64 비트

				EValueFormat_7C_F32 = 0x06220, 	// 채널 7 : 소수점 : 32 비트
				EValueFormat_7C_F64 = 0x06240, 	// 채널 7 : 소수점 : 64 비트
					
				EValueFormat_8C_U8 = 0x07008, 	// 채널 8 : 부호 미포함 : 8 비트
				EValueFormat_8C_U10 = 0x0700A,	// 채널 8 : 부호 미포함 : 10 비트
				EValueFormat_8C_U12 = 0x0700C,	// 채널 8 : 부호 미포함 : 12 비트
				EValueFormat_8C_U13 = 0x0700D,	// 채널 8 : 부호 미포함 : 13 비트
				EValueFormat_8C_U14 = 0x0700E,	// 채널 8 : 부호 미포함 : 14 비트
				EValueFormat_8C_U16 = 0x07010,	// 채널 8 : 부호 미포함 : 16 비트
				EValueFormat_8C_U32 = 0x07020,	// 채널 8 : 부호 미포함 : 32 비트
				EValueFormat_8C_U64 = 0x07040,	// 채널 8 : 부호 미포함 : 64 비트

				EValueFormat_8C_S8 = 0x07108,  	// 채널 8 : 부호 포함 : 8 비트
				EValueFormat_8C_S10 = 0x0710A, 	// 채널 8 : 부호 포함 : 10 비트
				EValueFormat_8C_S12 = 0x0710C, 	// 채널 8 : 부호 포함 : 12 비트
				EValueFormat_8C_S13 = 0x0710D, 	// 채널 8 : 부호 포함 : 13 비트
				EValueFormat_8C_S14 = 0x0710E, 	// 채널 8 : 부호 포함 : 14 비트
				EValueFormat_8C_S16 = 0x07110, 	// 채널 8 : 부호 포함 : 16 비트
				EValueFormat_8C_S32 = 0x07120, 	// 채널 8 : 부호 포함 : 32 비트
				EValueFormat_8C_S64 = 0x07140, 	// 채널 8 : 부호 포함 : 64 비트

				EValueFormat_8C_F32 = 0x07220,	// 채널 8 : 소수점 : 32 비트
				EValueFormat_8C_F64 = 0x07240,	// 채널 8 : 소수점 : 64 비트
					
				EValueFormat_9C_U8 = 0x08008,  	// 채널 9 : 부호 미포함 : 8 비트
				EValueFormat_9C_U10 = 0x0800A, 	// 채널 9 : 부호 미포함 : 10 비트
				EValueFormat_9C_U12 = 0x0800C, 	// 채널 9 : 부호 미포함 : 12 비트
				EValueFormat_9C_U13 = 0x0800D, 	// 채널 9 : 부호 미포함 : 13 비트
				EValueFormat_9C_U14 = 0x0800E, 	// 채널 9 : 부호 미포함 : 14 비트
				EValueFormat_9C_U16 = 0x08010, 	// 채널 9 : 부호 미포함 : 16 비트
				EValueFormat_9C_U32 = 0x08020, 	// 채널 9 : 부호 미포함 : 32 비트
				EValueFormat_9C_U64 = 0x08040, 	// 채널 9 : 부호 미포함 : 64 비트

				EValueFormat_9C_S8 = 0x08108, 	// 채널 9 : 부호 포함 : 8 비트
				EValueFormat_9C_S10 = 0x0810A,	// 채널 9 : 부호 포함 : 10 비트
				EValueFormat_9C_S12 = 0x0810C,	// 채널 9 : 부호 포함 : 12 비트
				EValueFormat_9C_S13 = 0x0810D,	// 채널 9 : 부호 포함 : 13 비트
				EValueFormat_9C_S14 = 0x0810E,	// 채널 9 : 부호 포함 : 14 비트
				EValueFormat_9C_S16 = 0x08110,	// 채널 9 : 부호 포함 : 16 비트
				EValueFormat_9C_S32 = 0x08120,	// 채널 9 : 부호 포함 : 32 비트
				EValueFormat_9C_S64 = 0x08140,	// 채널 9 : 부호 포함 : 64 비트

				EValueFormat_9C_F32 = 0x08220,	// 채널 9 : 소수점 : 32 비트
				EValueFormat_9C_F64 = 0x08240,	// 채널 9 : 소수점 : 64 비트
					
				EValueFormat_10C_U8 = 0x09008, 	// 채널 10 : 부호 미포함 : 8 비트
				EValueFormat_10C_U10 = 0x0900A,	// 채널 10 : 부호 미포함 : 10 비트
				EValueFormat_10C_U12 = 0x0900C,	// 채널 10 : 부호 미포함 : 12 비트
				EValueFormat_10C_U13 = 0x0900D,	// 채널 10 : 부호 미포함 : 13 비트
				EValueFormat_10C_U14 = 0x0900E,	// 채널 10 : 부호 미포함 : 14 비트
				EValueFormat_10C_U16 = 0x09010,	// 채널 10 : 부호 미포함 : 16 비트
				EValueFormat_10C_U32 = 0x09020,	// 채널 10 : 부호 미포함 : 32 비트
				EValueFormat_10C_U64 = 0x09040,	// 채널 10 : 부호 미포함 : 64 비트

				EValueFormat_10C_S8 = 0x09108, 	// 채널 10 : 부호 포함 : 8 비트
				EValueFormat_10C_S10 = 0x0910A,	// 채널 10 : 부호 포함 : 10 비트
				EValueFormat_10C_S12 = 0x0910C,	// 채널 10 : 부호 포함 : 12 비트
				EValueFormat_10C_S13 = 0x0910D,	// 채널 10 : 부호 포함 : 13 비트
				EValueFormat_10C_S14 = 0x0910E,	// 채널 10 : 부호 포함 : 14 비트
				EValueFormat_10C_S16 = 0x09110,	// 채널 10 : 부호 포함 : 16 비트
				EValueFormat_10C_S32 = 0x09120,	// 채널 10 : 부호 포함 : 32 비트
				EValueFormat_10C_S64 = 0x09140,	// 채널 10 : 부호 포함 : 64 비트

				EValueFormat_10C_F32 = 0x09220,	// 채널 10 : 소수점 : 32 비트
				EValueFormat_10C_F64 = 0x09240,	// 채널 10 : 소수점 : 64 비트
			};
			/**
			* FFT 정보를 표현합니다.
			*/
			typedef struct sFFTInfo
			{
				ptrdiff_t nOrgSizeX;
				ptrdiff_t nOrgSizeY;
				long nOrgWidthStep;
				bool bOrgColor;
				long nOrgDepth;
				long nOrgChannels;
				double dblFFTMaxValue;
				bool bFloating = false;

				EFFTMethod eMethod;
			}sFFTInfo;
			
			CRavidImage();
			CRavidImage(_In_ CRavidImage& imgInfo);
			CRavidImage(_In_ CRavidImage* pImgInfo);
			CRavidImage(_In_ ptrdiff_t nSizeX, _In_ ptrdiff_t nSizeY, _In_opt_ Framework::CMultipleVariable rmvFillValue = {}, _In_opt_ EValueFormat eValueFormat = EValueFormat_1C_U8, _In_opt_ long nWidthStep = 0, _In_opt_ long nAlignBytes = 4);  

			CRavidImage& operator=(_In_ CRavidImage& riInfo);
			virtual ~CRavidImage();
			
			/**
			* 이미지 데이터를 가지고 있는지 확인합니다.
			*
			* return value : bool 의 데이터 유무를 반환합니다.
			*/
			bool IsLoaded();
			
			/**
			* 이미지 파일을 읽어옵니다.
			*
			* param[in] lpszFileName : 파일명
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Load(_In_ LPCTSTR lpszFileName);
			
			/**
			* 이미지 파일로 저장합니다.
			*
			* param[in] lpszFileName : 파일명
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Save(_In_ LPCTSTR lpszFileName);
			
			/**
			* 이미지 파일(jpg)로 압축 저장합니다.
			*
			* param[in] lpszFileName : 파일명
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CompressionSave(_In_ LPCTSTR lpszFileName, _In_ int nCompressionQuality = 95);
			
			/**
			* 이미지 가로 크기를 가져옵니다.
			*
			* param[in] nSizeX : 이미지 가로 크기
			*/
			void SetSizeX(_In_ ptrdiff_t nSizeX);
			
			/**
			* 이미지 가로 크기를 가져옵니다.
			*
			* return value : ptrdiff_t 의 가로 크기를 반환합니다.
			*/
			ptrdiff_t GetSizeX();
			
			/**
			* 이미지 세로 크기를 가져옵니다.
			*
			* param[in] nSizeX : 이미지 세로 크기
			*/
			void SetSizeY(_In_ ptrdiff_t nSizeY);
			
			/**
			* 이미지 세로 크기를 가져옵니다.
			*
			* return value : ptrdiff_t 의 세로 크기를 반환합니다.
			*/
			ptrdiff_t GetSizeY();
			
			/**
			* 이미지 전체 데이터 크기(BYTE)를 가져옵니다.
			*
			* return value : ptrdiff_t 의 전체 데이터 크기(BYTE)를 반환합니다.
			*/
			ptrdiff_t GetImageSizeByte();
			
			/**
			* 컬러 이미지인지 확인합니다.
			*
			* return value : bool 의 컬러 이미지 여부를 반환합니다.
			*/
			bool IsColor();
			
			/**
			* 현재 이미지 정보를 소수점 유무로 변경합니다.
			*
			* param[in] bFloatingPoint : 소수점 사용 유무
			*
			* return value : bool 의 성공 유무를 반환합니다.
			*/
			bool SetFloatingPoint(_In_opt_ bool bFloatingPoint = false);
			
			/**
			* 현재 이미지를 소수점으로 표현하는지 확인합니다
			*
			* return value : bool 의 소수점 사용 유무를 반환합니다.
			*/
			bool IsFloatingPoint();
			
			/**
			* 이미지 가로 데이터 크기(BYTE)를 가져옵니다.
			*
			* param[in] nWidthStep : 이미지 세로 크기
			*/
			void SetWidthStep(_In_ ptrdiff_t nWidthStep);
			
			/**
			* 이미지 가로 데이터 크기(BYTE)를 가져옵니다.
			*
			* return value : ptrdiff_t 의 가로 데이터 크기(BYTE)를 반환합니다.
			*/
			ptrdiff_t GetWidthStep();
			
			/**
			* 이미지 픽셀 비트 수를 가져옵니다.
			*
			* return value : long 의 픽셀 표현 비트 수를 반환합니다.
			*/
			long GetDepth();
			
			/**
			* 이미지 픽셀 표현 채널 수를 가져옵니다.
			*
			* return value : long 의 픽셀 표현 채널 수를 반환합니다.
			*/
			long GetChannels();
			
			/**
			* 이미지 포맷을 가져옵니다.
			*
			* return value : EValueFormat 의 이미지 포맷을 반환합니다.
			*/
			EValueFormat GetValueFormat();
			
			/**
			* 입력된 정보로 이미지 포맷을 계산합니다.
			*
			* param[in_opt] nChannel : 이미지 픽셀 비트 수
			* param[in_opt] nBpp : 이미지 픽셀 비트
			* param[in_opt] eType : 이미지 데이터 타입
			*
			* return value : EValueFormat 의 이미지 포맷을 반환합니다.
			*/
			static EValueFormat MakeValueFormat(_In_opt_ long nChannel = 1, _In_opt_ long nBpp = 8, _In_opt_ EValueType eType = EValueType_Unsigned);
			
			/**
			* 이미지 가로 얼라인 단위를 설정합니다.
			*
			* param[in] nAlignBytes : 이미지 얼라인 단위
			*/
			void SetAlignBytes(_In_ long nAlignBytes);
			
			/**
			* 이미지 가로 얼라인 단위를 가져옵니다.
			*
			* return value : long 의 이미지 가로 얼라인 단위를 반환합니다.
			*/
			long GetAlignBytes();
			
			/**
			* 이미지 픽셀을 표현하는 크기(BYTE)를 가져옵니다.
			*
			* return value : long 의 픽셀을 표현하는 크기(BYTE)를 반환합니다.
			*/
			long GetPixelSizeByte();
			
			/**
			* 이미지 데이터 시작 포인터를 가져옵니다.
			*
			* return value : BYTE* 의 데이터 시작 포인터를 반환합니다.
			*/
			BYTE* GetBuffer();
			
			/**
			* 이미지 가로방향 픽셀 데이터 위치의 포인터 리스트를 가져옵니다.
			* 1 채널 8비트는 1 씩 증가된 값을 가짐
			* 3 채널 8비트는 3씩 증가된 값을 가짐
			* 4 채널 8비트는 4씩 증가된 값을 가짐
			*
			* return value : size_t* 의 데이터 위치의 포인터 리스트를 반환합니다.
			*/
			size_t* GetXOffsetTable();
			
			/**
			* 이미지 세로방향 위치별 픽셀 데이터 시작 포인터 리스트를 가져옵니다.
			*
			* return value : BYTE** 의 세로방향 위치별 데이터 시작 포인터 리스트를 반환합니다.
			*/
			BYTE** GetYOffsetTable();
			
			/**
			* 버퍼 클리어시 수동 해제 여부를 설정합니다.
			*
			* param[in] bManualDestroyBuffer : 버퍼 클리어시 수동 해제 여부
			*/
			void SetManualDestroyBuffer(_In_ bool bManualDestroyBuffer);
			
			/**
			* 버퍼 클리어시 수동 해제 여부를 가져옵니다.
			*
			* return value : bool 의 설정된 상태를 반환합니다.
			*/
			bool IsManualDestroyBuffer();
			
			/**
			* 적분 이미지 여부를 설정합니다.
			*
			* param[in] bIntegral : 적분 이미지 판단
			*/
			void SetIntegral(_In_ bool bIntegral);
			
			/**
			* 적분 이미지 여부를 가져옵니다.
			*
			* return value : bool 의 설정된 상태를 반환합니다.
			*/
			bool IsIntegral();
			
			/**
			* 파일 이름을 설정합니다.
			*
			* param[in] strFileName : 파일 이름
			*/
			void SetFileName(_In_ CString strFileName);
			
			/**
			* 파일 이름을 가져옵니다.
			*
			* return value : CString 의 파일 이름을 반환합니다.
			*/
			CString GetFileName();
			
			/**
			* 파일 경로를 설정합니다.
			*
			* param[in] strFilePathName : 파일 경로
			*/
			void SetFilePathName(_In_ CString strFilePathName);
			
			/**
			* 파일 경로를 가져옵니다.
			*
			* return value : CString 의 파일 경로를 반환합니다.
			*/
			CString GetFilePathName();
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] refGeometry : 계산 영역 (Reference)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidRect<int>& refGeometry, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] pGeometry : 계산 영역 (Pointer)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidRect<int>* pGeometry, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] refGeometry : 계산 영역 (Reference)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidRect<long long>& refGeometry, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] pGeometry : 계산 영역 (Pointer)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidRect<long long>* pGeometry, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] refGeometry : 계산 영역 (Reference)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidRect<float>& refGeometry, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] pGeometry : 계산 영역 (Pointer)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidRect<float>* pGeometry, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] refGeometry : 계산 영역 (Reference)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidRect<double>& refGeometry, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] pGeometry : 계산 영역 (Pointer)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidRect<double>* pGeometry, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] refGeometry : 계산 영역 (Reference)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ RECT& refGeometry, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] pGeometry : 계산 영역 (Pointer)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ RECT* pGeometry, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] refPoint1 : 시작 위치 (Reference)
			* param[in] refPoint2 : 끝 위치 (Reference)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidPoint<int>& refPoint1, _In_ CRavidPoint<int>& refPoint2, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] refPoint1 : 시작 위치 (Reference)
			* param[in] refPoint2 : 끝 위치 (Reference)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidPoint<float>& refPoint1, _In_ CRavidPoint<float>& refPoint2, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] refPoint1 : 시작 위치 (Reference)
			* param[in] refPoint2 : 끝 위치 (Reference)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidPoint<long long>& refPoint1, _In_ CRavidPoint<long long>& refPoint2, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] refPoint1 : 시작 위치 (Reference)
			* param[in] refPoint2 : 끝 위치 (Reference)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidPoint<double>& refPoint1, _In_ CRavidPoint<double>& refPoint2, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] pPoint1 : 시작 위치 (Pointer)
			* param[in] pPoint2 : 끝 위치 (Pointer)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidPoint<int>* pPoint1, _In_ CRavidPoint<int>* pPoint2, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] pPoint1 : 시작 위치 (Pointer)
			* param[in] pPoint2 : 끝 위치 (Pointer)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidPoint<float>* pPoint1, _In_ CRavidPoint<float>* pPoint2, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] pPoint1 : 시작 위치 (Pointer)
			* param[in] pPoint2 : 끝 위치 (Pointer)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidPoint<long long>* pPoint1, _In_ CRavidPoint<long long>* pPoint2, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] pPoint1 : 시작 위치 (Pointer)
			* param[in] pPoint2 : 끝 위치 (Pointer)
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ CRavidPoint<double>* pPoint1, _In_ CRavidPoint<double>* pPoint2, _Out_ Framework::CMultipleVariable* rmvSumValue);
						
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] left : x 축 시작
			* param[in] top : y 축 시작
			* param[in] right : x 축 끝
			* param[in] bottom : y 축 끝
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ int left, _In_ int top, _In_ int right, _In_ int bottom, _Out_ Framework::CMultipleVariable* rmvSumValue);
						
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] left : x 축 시작
			* param[in] top : y 축 시작
			* param[in] right : x 축 끝
			* param[in] bottom : y 축 끝
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ float left, _In_ float top, _In_ float right, _In_ float bottom, _Out_ Framework::CMultipleVariable* rmvSumValue);
						
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] left : x 축 시작
			* param[in] top : y 축 시작
			* param[in] right : x 축 끝
			* param[in] bottom : y 축 끝
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ long long left, _In_ long long top, _In_ long long right, _In_ long long bottom, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 적분 이미지에서 입력 영역의 크기를 계산합니다.
			*
			* param[in] left : x 축 시작
			* param[in] top : y 축 시작
			* param[in] right : x 축 끝
			* param[in] bottom : y 축 끝
			* param[out] rmvSumValue : 결과 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetIntegralValue(_In_ double left, _In_ double top, _In_ double right, _In_ double bottom, _Out_ Framework::CMultipleVariable* rmvSumValue);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ BYTE* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 가져옵니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ BYTE* pValue1, _Out_ BYTE* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 가져옵니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ BYTE* pValue1, _Out_ BYTE* pValue2, _Out_ BYTE* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 가져옵니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ BYTE* pValue1, _Out_ BYTE* pValue2, _Out_ BYTE* pValue3, _Out_ BYTE* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ WORD* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 가져옵니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ WORD* pValue1, _Out_ WORD* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 가져옵니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ WORD* pValue1, _Out_ WORD* pValue2, _Out_ WORD* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 가져옵니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ WORD* pValue1, _Out_ WORD* pValue2, _Out_ WORD* pValue3, _Out_ WORD* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ DWORD* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 가져옵니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ DWORD* pValue1, _Out_ DWORD* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 가져옵니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ DWORD* pValue1, _Out_ DWORD* pValue2, _Out_ DWORD* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 가져옵니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ DWORD* pValue1, _Out_ DWORD* pValue2, _Out_ DWORD* pValue3, _Out_ DWORD* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ float* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 가져옵니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ float* pValue1, _Out_ float* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 가져옵니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ float* pValue1, _Out_ float* pValue2, _Out_ float* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 가져옵니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ float* pValue1, _Out_ float* pValue2, _Out_ float* pValue3, _Out_ float* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ double* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 가져옵니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ double* pValue1, _Out_ double* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 가져옵니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ double* pValue1, _Out_ double* pValue2, _Out_ double* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 가져옵니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ double* pValue1, _Out_ double* pValue2, _Out_ double* pValue3, _Out_ double* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 long long의 값으로 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ long long* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 long long의 값으로 가져옵니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ long long* pValue1, _Out_ long long* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 long long의 값으로 가져옵니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ long long* pValue1, _Out_ long long* pValue2, _Out_ long long* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 long long의 값으로 가져옵니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ long long* pValue1, _Out_ long long* pValue2, _Out_ long long* pValue3, _Out_ long long* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 unsigned long long의 값으로 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ unsigned long long* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 unsigned long long의 값으로 가져옵니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ unsigned long long* pValue1, _Out_ unsigned long long* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 unsigned long long의 값으로 가져옵니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ unsigned long long* pValue1, _Out_ unsigned long long* pValue2, _Out_ unsigned long long* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 unsigned long long의 값으로 가져옵니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*/
			void GetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ unsigned long long* pValue1, _Out_ unsigned long long* pValue2, _Out_ unsigned long long* pValue3, _Out_ unsigned long long* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ BYTE* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ BYTE* pValue1, _Out_ BYTE* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ BYTE* pValue1, _Out_ BYTE* pValue2, _Out_ BYTE* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ BYTE* pValue1, _Out_ BYTE* pValue2, _Out_ BYTE* pValue3, _Out_ BYTE* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ WORD* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ WORD* pValue1, _Out_ WORD* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ WORD* pValue1, _Out_ WORD* pValue2, _Out_ WORD* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ WORD* pValue1, _Out_ WORD* pValue2, _Out_ WORD* pValue3, _Out_ WORD* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ DWORD* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ DWORD* pValue1, _Out_ DWORD* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ DWORD* pValue1, _Out_ DWORD* pValue2, _Out_ DWORD* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ DWORD* pValue1, _Out_ DWORD* pValue2, _Out_ DWORD* pValue3, _Out_ DWORD* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ float* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ float* pValue1, _Out_ float* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ float* pValue1, _Out_ float* pValue2, _Out_ float* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ float* pValue1, _Out_ float* pValue2, _Out_ float* pValue3, _Out_ float* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ double* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ double* pValue1, _Out_ double* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ double* pValue1, _Out_ double* pValue2, _Out_ double* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ double* pValue1, _Out_ double* pValue2, _Out_ double* pValue3, _Out_ double* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 long long의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ long long* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 long long의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ long long* pValue1, _Out_ long long* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 long long의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ long long* pValue1, _Out_ long long* pValue2, _Out_ long long* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 long long의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ long long* pValue1, _Out_ long long* pValue2, _Out_ long long* pValue3, _Out_ long long* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 unsigned long long의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ unsigned long long* pValue1);

			/**
			* 이미지에서 입력된 위치의 데이터를 unsigned long long의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 2개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ unsigned long long* pValue1, _Out_ unsigned long long* pValue2);

			/**
			* 이미지에서 입력된 위치의 데이터를 unsigned long long의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 3개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ unsigned long long* pValue1, _Out_ unsigned long long* pValue2, _Out_ unsigned long long* pValue3);

			/**
			* 이미지에서 입력된 위치의 데이터를 unsigned long long의 값으로 가져옵니다.
			* x, y 의 범위를 확인하기 때문에 GetValue 보다 느립니다.
			* 연속된 4개의 값을 가져옵니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[out] pValue1 : 데이터 값 (Pointer)
			* param[out] pValue2 : pValue1 다음 데이터 값 (Pointer)
			* param[out] pValue3 : pValue2 다음 데이터 값 (Pointer)
			* param[out] pValue4 : pValue3 다음 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ unsigned long long* pValue1, _Out_ unsigned long long* pValue2, _Out_ unsigned long long* pValue3, _Out_ unsigned long long* pValue4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ BYTE value1);

			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 설정합니다.
			* 연속된 2개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ BYTE value1, _In_ BYTE value2);

			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 설정합니다.
			* 연속된 3개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ BYTE value1, _In_ BYTE value2, _In_ BYTE value3);

			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 설정합니다.
			* 연속된 4개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			* param[in] value4 : value3 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ BYTE value1, _In_ BYTE value2, _In_ BYTE value3, _In_ BYTE value4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ WORD value1);

			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 설정합니다.
			* 연속된 2개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ WORD value1, _In_ WORD value2);

			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 설정합니다.
			* 연속된 3개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ WORD value1, _In_ WORD value2, _In_ WORD value3);

			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 설정합니다.
			* 연속된 4개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			* param[in] value4 : value3 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ WORD value1, _In_ WORD value2, _In_ WORD value3, _In_ WORD value4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ DWORD value1);

			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 설정합니다.
			* 연속된 2개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ DWORD value1, _In_ DWORD value2);

			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 설정합니다.
			* 연속된 3개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ DWORD value1, _In_ DWORD value2, _In_ DWORD value3);

			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 설정합니다.
			* 연속된 4개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			* param[in] value4 : value3 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ DWORD value1, _In_ DWORD value2, _In_ DWORD value3, _In_ DWORD value4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ float value1);

			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 설정합니다.
			* 연속된 2개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ float value1, _In_ float value2);

			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 설정합니다.
			* 연속된 3개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ float value1, _In_ float value2, _In_ float value3);

			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 설정합니다.
			* 연속된 4개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			* param[in] value4 : value3 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ float value1, _In_ float value2, _In_ float value3, _In_ float value4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ double value1);

			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 설정합니다.
			* 연속된 2개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ double value1, _In_ double value2);

			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 설정합니다.
			* 연속된 3개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ double value1, _In_ double value2, _In_ double value3);

			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 설정합니다.
			* 연속된 4개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			* param[in] value4 : value3 다음 데이터 값
			*/
			void SetValue(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ double value1, _In_ double value2, _In_ double value3, _In_ double value4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ BYTE value1);

			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 2개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ BYTE value1, _In_ BYTE value2);

			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 3개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ BYTE value1, _In_ BYTE value2, _In_ BYTE value3);

			/**
			* 이미지에서 입력된 위치의 데이터를 BYTE의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 4개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			* param[in] value4 : value3 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ BYTE value1, _In_ BYTE value2, _In_ BYTE value3, _In_ BYTE value4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ WORD value1);

			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 2개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ WORD value1, _In_ WORD value2);

			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 3개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ WORD value1, _In_ WORD value2, _In_ WORD value3);

			/**
			* 이미지에서 입력된 위치의 데이터를 WORD의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 4개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			* param[in] value4 : value3 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ WORD value1, _In_ WORD value2, _In_ WORD value3, _In_ WORD value4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ DWORD value1);

			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 2개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ DWORD value1, _In_ DWORD value2);

			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 3개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ DWORD value1, _In_ DWORD value2, _In_ DWORD value3);

			/**
			* 이미지에서 입력된 위치의 데이터를 DWORD의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 4개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			* param[in] value4 : value3 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ DWORD value1, _In_ DWORD value2, _In_ DWORD value3, _In_ DWORD value4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ float value1);

			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 2개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ float value1, _In_ float value2);

			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 3개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ float value1, _In_ float value2, _In_ float value3);

			/**
			* 이미지에서 입력된 위치의 데이터를 float의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 4개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			* param[in] value4 : value3 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ float value1, _In_ float value2, _In_ float value3, _In_ float value4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ double value1);

			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 2개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ double value1, _In_ double value2);

			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 3개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ double value1, _In_ double value2, _In_ double value3);

			/**
			* 이미지에서 입력된 위치의 데이터를 double의 값으로 설정합니다.
			* x, y 의 범위를 확인하기 때문에 SetValue 보다 느립니다.
			* 연속된 4개의 값을 설정합니다.
			*
			* param[in] x : x 축 시작
			* param[in] y : y 축 시작
			* param[in] value1 : 데이터 값
			* param[in] value2 : value1 다음 데이터 값
			* param[in] value3 : value2 다음 데이터 값
			* param[in] value4 : value3 다음 데이터 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetValueSafe(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _In_ double value1, _In_ double value2, _In_ double value3, _In_ double value4);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 선형 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] x : x 축 좌표
			* param[in] y : y 축 좌표
			*
			* return value : float 의 보간 데이터를 반환합니다.
			*/
			float GetGVBilinear(_In_ float x, _In_ float y);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 선형 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] pPoint : x, y 축 좌표 (Pointer)
			*
			* return value : float 의 보간 데이터를 반환합니다.
			*/
			float GetGVBilinear(_In_ CRavidPoint<float>* pPoint);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 선형 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] refPoint : x, y 축 좌표 (Reference)
			*
			* return value : float 의 보간 데이터를 반환합니다.
			*/
			float GetGVBilinear(_In_ CRavidPoint<float>& refPoint);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 선형 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] pPoint : x, y 축 좌표 (Pointer)
			*
			* return value : float 의 보간 데이터를 반환합니다.
			*/
			float GetGVBilinear(_In_ CRavidPoint<double>* pPoint);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 선형 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] refPoint : x, y 축 좌표 (Reference)
			*
			* return value : float 의 보간 데이터를 반환합니다.
			*/
			float GetGVBilinear(_In_ CRavidPoint<double>& refPoint);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 선형 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] x : x 축 좌표
			* param[in] y : y 축 좌표
			* param[out] pR : 보간 데이터 값 (Pointer)
			* param[out] pG : 보간 데이터 값 (Pointer)
			* param[out] pB : 보간 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetRGBBilinear(_In_ float x, _In_ float y, _Out_ float* pR, _Out_ float* pG, _Out_ float* pB);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 선형 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] pPoint : x, y 축 좌표 (Pointer)
			* param[out] pR : 보간 데이터 값 (Pointer)
			* param[out] pG : 보간 데이터 값 (Pointer)
			* param[out] pB : 보간 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetRGBBilinear(_In_ CRavidPoint<float>* pPoint, _Out_ float* pR, _Out_ float* pG, _Out_ float* pB);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 선형 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] refPoint : x, y 축 좌표 (Reference)
			* param[out] pR : 보간 데이터 값 (Pointer)
			* param[out] pG : 보간 데이터 값 (Pointer)
			* param[out] pB : 보간 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetRGBBilinear(_In_ CRavidPoint<float>& refPoint, _Out_ float* pR, _Out_ float* pG, _Out_ float* pB);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 선형 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] pPoint : x, y 축 좌표 (Pointer)
			* param[out] pR : 보간 데이터 값 (Pointer)
			* param[out] pG : 보간 데이터 값 (Pointer)
			* param[out] pB : 보간 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetRGBBilinear(_In_ CRavidPoint<double>* pPoint, _Out_ float* pR, _Out_ float* pG, _Out_ float* pB);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 선형 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] refPoint : x, y 축 좌표 (Reference)
			* param[out] pR : 보간 데이터 값 (Pointer)
			* param[out] pG : 보간 데이터 값 (Pointer)
			* param[out] pB : 보간 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetRGBBilinear(_In_ CRavidPoint<double>& refPoint, _Out_ float* pR, _Out_ float* pG, _Out_ float* pB);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 3차 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] x : x 축 좌표
			* param[in] y : y 축 좌표
			*
			* return value : float 의 보간 데이터를 반환합니다.
			*/
			float GetGVBicubic(_In_ float x, _In_ float y);
			/**
			* 이미지에서 입력된 위치의 데이터를 3차 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] pPoint : x, y 축 좌표 (Pointer)
			*
			* return value : float 의 보간 데이터를 반환합니다.
			*/
			float GetGVBicubic(_In_ CRavidPoint<float>* pPoint);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 3차 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] refPoint : x, y 축 좌표 (Reference)
			*
			* return value : float 의 보간 데이터를 반환합니다.
			*/
			float GetGVBicubic(_In_ CRavidPoint<float>& refPoint);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 3차 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] pPoint : x, y 축 좌표 (Pointer)
			*
			* return value : float 의 보간 데이터를 반환합니다.
			*/
			float GetGVBicubic(_In_ CRavidPoint<double>* pPoint);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 3차 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] refPoint : x, y 축 좌표 (Reference)
			*
			* return value : float 의 보간 데이터를 반환합니다.
			*/
			float GetGVBicubic(_In_ CRavidPoint<double>& refPoint);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 3차 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] x : x 축 좌표
			* param[in] y : y 축 좌표
			* param[out] pR : 보간 데이터 값 (Pointer)
			* param[out] pG : 보간 데이터 값 (Pointer)
			* param[out] pB : 보간 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetRGBBicubic(_In_ float x, _In_ float y, _Out_ float* pR, _Out_ float* pG, _Out_ float* pB);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 3차 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] pPoint : x, y 축 좌표 (Pointer)
			* param[out] pR : 보간 데이터 값 (Pointer)
			* param[out] pG : 보간 데이터 값 (Pointer)
			* param[out] pB : 보간 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetRGBBicubic(_In_ CRavidPoint<float>* pPoint, _Out_ float* pR, _Out_ float* pG, _Out_ float* pB);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 3차 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] refPoint : x, y 축 좌표 (Reference)
			* param[out] pR : 보간 데이터 값 (Pointer)
			* param[out] pG : 보간 데이터 값 (Pointer)
			* param[out] pB : 보간 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetRGBBicubic(_In_ CRavidPoint<float>& refPoint, _Out_ float* pR, _Out_ float* pG, _Out_ float* pB);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 3차 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] pPoint : x, y 축 좌표 (Pointer)
			* param[out] pR : 보간 데이터 값 (Pointer)
			* param[out] pG : 보간 데이터 값 (Pointer)
			* param[out] pB : 보간 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetRGBBicubic(_In_ CRavidPoint<double>* pPoint, _Out_ float* pR, _Out_ float* pG, _Out_ float* pB);
			
			/**
			* 이미지에서 입력된 위치의 데이터를 3차 보간하여 float의 값으로 가져옵니다.
			*
			* param[in] refPoint : x, y 축 좌표 (Reference)
			* param[out] pR : 보간 데이터 값 (Pointer)
			* param[out] pG : 보간 데이터 값 (Pointer)
			* param[out] pB : 보간 데이터 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetRGBBicubic(_In_ CRavidPoint<double>& refPoint, _Out_ float* pR, _Out_ float* pG, _Out_ float* pB);
			
			/**
			* 현재 클래스를 초기화합니다.
			*/
			void Clear();
			
			/**
			* 입력된 이미지를 현재 이미지에 복사합니다.
			*
			* param[in] refSrcImage : 입력 이미지 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Copy(_In_ CRavidImage& refSrcImage);
			
			/**
			* 입력된 이미지를 현재 이미지에 복사합니다.
			*
			* param[in] pSrcImage : 입력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Copy(_In_ CRavidImage* pSrcImage);
			
			/**
			* 현재 이미지를 입력된 이미지에 붙여넣기 합니다.
			*
			* param[out] refDstImage : 출력 이미지 (Reference)
			* param[in_opt] pSrcRegion : 복사 영역 (Pointer)
			* param[in_opt] pDstRegion : 붙여넣기 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Paste(_Out_ CRavidImage& refDstImage, _In_opt_ CRavidGeometry* pSrcRegion = nullptr, _In_opt_ CRavidGeometry* pDstRegion = nullptr);
			
			/**
			* 현재 이미지를 입력된 이미지에 붙여넣기 합니다.
			*
			* param[out] pDstImage : 출력 이미지 (Pointer)
			* param[in_opt] pSrcRegion : 복사 영역 (Pointer)
			* param[in_opt] pDstRegion : 붙여넣기 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Paste(_Out_ CRavidImage* pDstImage, _In_opt_ CRavidGeometry* pSrcRegion = nullptr, _In_opt_ CRavidGeometry* pDstRegion = nullptr);
			
			/**
			* 입력된 이미지와 현재 이미지를 교체합니다.
			*
			* param[in_out] refDstImage : 교체 이미지 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Swap(_Inout_ CRavidImage& refDstImage);
			
			/**
			* 입력된 이미지와 현재 이미지를 교체합니다.
			*
			* param[in_out] pDstImage : 교체 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Swap(_Inout_ CRavidImage* pDstImage);
			
			/**
			* 입력된 3채널 이미지를 현재 이미지에 1채널 평균 이미지로 복사합니다.
			*
			* param[in] pSrcImageRGB : 입력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CopyRGBToGray(_In_ CRavidImage* pSrcImageRGB);
			
			/**
			* 입력된 4채널 이미지를 현재 이미지에 1채널 평균 이미지로 복사합니다.
			*
			* param[in] pSrcImageARGB : 입력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CopyARGBToGray(_In_ CRavidImage* pSrcImageARGB);
			
			/**
			* 입력된 1채널 이미지를 현재 이미지에 3채널 RGB이미지로 복사합니다.
			*
			* param[in] pSrcImageGray : 입력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CopyGrayToRGB(_In_ CRavidImage* pSrcImageGray);
			
			/**
			* 입력된 1채널 이미지를 현재 이미지에 4채널 ARGB이미지로 복사합니다.
			*
			* param[in] pSrcImageGray : 입력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CopyGrayToARGB(_In_ CRavidImage* pSrcImageGray);
			
			/**
			* 입력된 3채널 이미지를 현재 이미지에 1채널 Gray 이미지로 복사합니다.
			*
			* param[in] pSrcImageRGB : 입력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CopyRGBToYComponent(_In_ CRavidImage* pSrcImageRGB);
			
			/**
			* 입력된 4채널 이미지를 현재 이미지에 1채널 Gray 이미지로 복사합니다.
			*
			* param[in] pSrcImageARGB : 입력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CopyARGBToYComponent(_In_ CRavidImage* pSrcImageARGB);
			
			/**
			* 입력된 3채널 이미지를 현재 이미지에 4채널 ARGB이미지로 복사합니다.
			*
			* param[in] pSrcImageRGB : 입력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CopyRGBToARGB(_In_ CRavidImage* pSrcImageRGB);
			
			/**
			* 입력된 4채널 이미지를 현재 이미지에 3채널 RGB이미지로 복사합니다.
			*
			* param[in] pSrcImageRGB : 입력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CopyARGBToRGB(_In_ CRavidImage* pSrcImageARGB);
			
			/**
			* 입력된 정보로 이미지를 생성합니다.
			*
			* param[in] nSizeX : 이미지 가로 크기
			* param[in] nSizeY : 이미지 세로 크기
			* param[in_opt] rmvFillValue : 픽셀 설정 초기값
			* param[in_opt] eValueFormat : 채널 및 표현, 픽셀 비트
			* param[in_opt] nWidthStep : 이미지 가로 데이터 크기
			* param[in_opt] nAlignBytes : 이미지 가로 얼라인 단위
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CreateImageBuffer(_In_ ptrdiff_t nSizeX, _In_ ptrdiff_t nSizeY, _In_opt_ Framework::CMultipleVariable rmvFillValue = {}, _In_opt_ EValueFormat eValueFormat = EValueFormat_1C_U8, _In_opt_ long nWidthStep = 0, _In_opt_ long nAlignBytes = 4);
			
			/**
			* 입력된 정보로 이미지를 얕은 복사합니다.
			*
			* param[in] pData : 이미지 데이터 리스트 (Pointer)
			* param[in] nSizeX : 이미지 가로 크기
			* param[in] nSizeY : 이미지 세로 크기
			* param[in_opt] eValueFormat : 채널 및 표현, 픽셀 비트
			* param[in_opt] nWidthStep : 이미지 가로 데이터 크기
			* param[in_opt] nAlignBytes : 이미지 가로 얼라인 단위
			* param[in_opt] bManualDestroy : 이미지 수동 삭제
			* param[in_opt] bClear : 생성전 초기화 여부
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetImagePtr(_In_ BYTE* pData, _In_ ptrdiff_t nSizeX, _In_ ptrdiff_t nSizeY, _In_opt_ EValueFormat eValueFormat = EValueFormat_1C_U8, _In_opt_ long nWidthStep = 0, _In_opt_ long nAlignBytes = 4, _In_opt_ bool bManualDestroy = true, _In_opt_ bool bClear = true);
			
			/**
			* 3채널 현재 이미지를 1채널 평균 이미지로 변환합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToGray();
			
			/**
			* 4채널 현재 이미지를 1채널 평균 이미지로 변환합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertARGBToGray();
			
			/**
			* 1채널 현재 이미지를 3채널 RGB 이미지로 변환합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertGrayToRGB();
			
			/**
			* 1채널 현재 이미지를 3채널 ARGB 이미지로 변환합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertGrayToARGB();
			
			/**
			* 3채널 현재 이미지를 1채널 Gray 이미지로 변환합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToYComponent();
			
			/**
			* 4채널 현재 이미지를 1채널 Gray 이미지로 변환합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertARGBToYComponent();
			
			/**
			* 3채널 현재 이미지를 4채널 ARGB 이미지로 변환합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertRGBToARGB();
			
			/**
			* 4채널 현재 이미지를 3채널 RGB 이미지로 변환합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertARGBToRGB();
			
			/**
			* 입력된 정보로 이미지를 회전합니다.
			*
			* param[in] dblAngle : 회전 각도
			* param[in_opt] pPivot : 회전 중심 위치 (Pointer)
			* param[in_opt] rmvFillValue : 빈 공각 채우는 값
			* param[in_opt] bResizing : 회전 후 이미지 크기 재 설정
			* param[in_opt] eMethod : 회전 시 데이터 보간 방법
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Rotate(_In_ double dblAngle, _In_opt_ CRavidPoint<double>* pPivot = nullptr, _In_opt_ Framework::CMultipleVariable rmvFillValue = 0, _In_opt_ bool bResizing = false, _In_opt_ EInterpolationMethod eMethod = EInterpolationMethod_Bilinear, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력된 정보로 이미지의 크기를 변경합니다.
			*
			* param[in] refRatio : 변경 xy 비율 (Reference)
			* param[in_opt] bResizing : 변경 후 이미지 크기 재 설정
			* param[in_opt] pPivot : 크기 변환 중심 위치 (Pointer)
			* param[in_opt] rmvFillValue : 빈 공각 채우는 값
			* param[in_opt] eMethod : 회전 시 데이터 보간 방법
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Scale(_In_ CRavidPoint<double>& refRatio, _In_opt_ bool bResizing = true, _In_opt_ CRavidPoint<double>* pPivot = nullptr, _In_opt_ Framework::CMultipleVariable rmvFillValue = 0, _In_opt_ EInterpolationMethod eMethod = EInterpolationMethod_Bilinear, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력된 정보로 이미지의 크기를 변경합니다.
			*
			* param[in] pRatio : 변경 xy 비율 (Pointer)
			* param[in_opt] bResizing : 변경 후 이미지 크기 재 설정
			* param[in_opt] pPivot : 크기 변환 중심 위치 (Pointer)
			* param[in_opt] rmvFillValue : 빈 공각 채우는 값
			* param[in_opt] eMethod : 회전 시 데이터 보간 방법
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Scale(_In_ CRavidPoint<double>* pRatio, _In_opt_ bool bResizing = true, _In_opt_ CRavidPoint<double>* pPivot = nullptr, _In_opt_ Framework::CMultipleVariable rmvFillValue = 0, _In_opt_ EInterpolationMethod eMethod = EInterpolationMethod_Bilinear, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력된 정보로 이미지의 크기를 변경합니다.
			*
			* param[in] ratioX : 변경 x 비율
			* param[in] ratioY : 변경 y 비율
			* param[in_opt] bResizing : 변경 후 이미지 크기 재 설정
			* param[in_opt] pPivot : 크기 변환 중심 위치 (Pointer)
			* param[in_opt] rmvFillValue : 빈 공각 채우는 값
			* param[in_opt] eMethod : 회전 시 데이터 보간 방법
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Scale(_In_ double ratioX, _In_ double ratioY, _In_opt_ bool bResizing = true, _In_opt_ CRavidPoint<double>* pPivot = nullptr, _In_opt_ Framework::CMultipleVariable rmvFillValue = 0, _In_opt_ EInterpolationMethod eMethod = EInterpolationMethod_Bilinear, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력된 정보로 현재 이미지를 이동합니다.
			*
			* param[in] deltaX : 이동 x 거리
			* param[in] deltaY : 이동 y 거리
			* param[in_opt] rmvFillValue : 빈 공각 채우는 값
			* param[in_opt] eMethod : 회전 시 데이터 보간 방법
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult MovePosition(_In_ double deltaX, _In_ double deltaY, _In_opt_ Framework::CMultipleVariable rmvFillValue = 0, _In_opt_ EInterpolationMethod eMethod = EInterpolationMethod_Bilinear, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력된 정보로 현재 이미지를 이동합니다.
			*
			* param[in] refDelta : 이동 xy 거리 (Reference)
			* param[in_opt] rmvFillValue : 빈 공각 채우는 값
			* param[in_opt] eMethod : 회전 시 데이터 보간 방법
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult MovePosition(_In_ CRavidPoint<double>& refDelta, _In_opt_ Framework::CMultipleVariable rmvFillValue = 0, _In_opt_ EInterpolationMethod eMethod = EInterpolationMethod_Bilinear, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력된 정보로 현재 이미지를 이동합니다.
			*
			* param[in] pDelta : 이동 xy 거리 (Pointer)
			* param[in_opt] rmvFillValue : 빈 공각 채우는 값
			* param[in_opt] eMethod : 회전 시 데이터 보간 방법
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult MovePosition(_In_ CRavidPoint<double>* pDelta, _In_opt_ Framework::CMultipleVariable rmvFillValue = 0, _In_opt_ EInterpolationMethod eMethod = EInterpolationMethod_Bilinear, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력된 데이터 값으로 현재 데이터의 값을 비율만큼 곱한 후 입력 값을 더합니다.
			*
			* param[in] rmvGainVal : 밝기 변환 비율
			* param[in] pGeometry : 적용 영역 (Pointer)
			* param[in] rmvOffsetVal : 밝기 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Gain(_In_ Framework::CMultipleVariable rmvGainVal, _In_ CRavidGeometry* pGeometry, _In_ Framework::CMultipleVariable rmvOffsetVal);
			
			/**
			* 입력된 데이터 값으로 현재 데이터의 값을 비율만큼 곱한 후 입력 값을 더합니다.
			*
			* param[in] rmvGainVal : 밝기 변환 비율
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			* param[in_opt] pmvOffsetVal : 밝기 값 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Gain(_In_ Framework::CMultipleVariable rmvGainVal, _In_opt_ CRavidGeometry* pGeometry = nullptr, _In_opt_ Framework::CMultipleVariable* pmvOffsetVal = nullptr);
			
			/**
			* 입력 값으로 현재 이미지 데이터에 더합니다.
			*
			* param[in] rmvOffsetVal : 밝기 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Offset(_In_ Framework::CMultipleVariable rmvOffsetVal, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지 데이터에 컨볼루션을 합니다.
			*
			* param[in] rmKernel : 연산 메트릭스 (Reference)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			* param[in_opt] bAbsolute : 절대값 설정
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convolution(_In_ Ravid::Mathematics::CMatrix& rmKernel, _In_opt_ CRavidGeometry* pGeometry = nullptr, _In_opt_ bool bAbsolute = false);
			
			/**
			* 입력 값으로 현재 이미지 데이터에 컨볼루션을 합니다.
			*
			* param[in] pRmKernel : 연산 메트릭스 (Pointer)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			* param[in_opt] bAbsolute : 절대값 설정
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convolution(_In_ Ravid::Mathematics::CMatrix* pRmKernel, _In_opt_ CRavidGeometry* pGeometry = nullptr, _In_opt_ bool bAbsolute = false);
			
			/**
			* 입력 값으로 현재 이미지 데이터에 컨볼루션을 합니다.
			*
			* param[in] ePreset : 연산 프리셋
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Convolution(_In_ EConvolutionPreset ePreset, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지 데이터에 지정된 연산을 합니다.
			*
			* param[in] refOperation : 연산 정보 (Reference)
			* param[in_opt] pRgSrcRegion : 원본 영역 (Pointer)
			* param[in_opt] pRgOperRegion : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Operation(_In_ CImageOperationInfo& refOperation, _In_opt_ CRavidGeometry* pRgSrcRegion = nullptr, _In_opt_ CRavidGeometry* pRgOperRegion = nullptr);
			
			/**
			* 입력 값으로 현재 이미지 데이터에 지정된 연산을 합니다.
			*
			* param[in] pOperation : 연산 정보 (Pointer)
			* param[in_opt] pRgSrcRegion : 원본 영역 (Pointer)
			* param[in_opt] pRgOperRegion : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Operation(_In_ CImageOperationInfo* pOperation, _In_opt_ CRavidGeometry* pRgSrcRegion = nullptr, _In_opt_ CRavidGeometry* pRgOperRegion = nullptr);
			
			/**
			* 입력 값으로 현재 이미지 데이터를 설정 합니다.
			*
			* param[in] rmvFill : 설정할 값
			* param[in] refGeometry : 적용 영역 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Mask(_In_ Framework::CMultipleVariable rmvFill, _In_ CRavidGeometry& refGeometry);
			
			/**
			* 입력 값으로 현재 이미지를 설정 합니다.
			*
			* param[in] rmvFill : 설정할 값
			* param[in] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Mask(_In_ Framework::CMultipleVariable rmvFill, _In_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지를 반전합니다.
			*
			* param[in] eFlipType : 반전 방법
			* param[in_opt] pPivot : 반전 중심 좌표 (Pointer)
			* param[in_opt] rmvFillValue : 빈 공간 채울 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Flip(_In_ EFlipType eFlipType, _In_opt_ CRavidPoint<double>* pPivot = nullptr, _In_opt_ Framework::CMultipleVariable rmvFillValue = 0, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지 영역만 잘라냅니다.
			*
			* param[in] refGeometry : 적용 영역 (Reference)
			* param[in_opt] rmvFillValue : 빈 공간 채울 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Crop(_In_ CRavidGeometry& refGeometry, _In_opt_ Framework::CMultipleVariable rmvFillValue = 0);
			
			/**
			* 입력 값으로 현재 이미지 영역만 잘라냅니다.
			*
			* param[in] pGeometry : 적용 영역 (Pointer)
			* param[in_opt] rmvFillValue : 빈 공간 채울 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Crop(_In_ CRavidGeometry* pGeometry, _In_opt_ Framework::CMultipleVariable rmvFillValue = 0);
			
			/**
			* 입력 값으로 현재 이미지에 헤리스코너 알고리즘을 합니다.
			*
			* param[in] ehcdtType : 헤리스코너 이진화 방법 설정
			* param[in] fThreshHold : 이진화 값
			* param[out] pGeometryArray : 특징 좌표 (Pointer)
			* param[in_opt] fCornerCoefficient : 특징 계수
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult HarrisCornerDetector(_In_ EHarrisCornerDetectorThresholdType ehcdtType, _In_ float fThreshHold, _Out_ CRavidGeometryArray *pGeometryArray, _In_opt_ float fCornerCoefficient = 0.04f, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에 케니 엣지 알고리즘을 합니다.
			*
			* param[in] fLowThreshold : 최소 이진화 값
			* param[in] fHighThreshold : 최대 이진화 값
			* param[in_opt] ecedtType : 케니 엣지 방법 설정
			* param[in_opt] nGaussianBlurSize : 가우시안 크기 설정
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CannyEdgeDetector(_In_ float fLowThreshold, _In_ float fHighThreshold, _In_opt_ ECannyEdgeDetectorThresholdType ecedtType = ECannyEdgeDetectorThresholdType_Absolute, _In_opt_ int nGaussianBlurSize = 0, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 프로젝션 알고리즘을 합니다.
			*
			* param[in] eprjDir : 프로젝션 방향 설정
			* param[out] pVctResult : 분석 결과
			* param[in_opt] dblAngle : 측정 각도
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Projection(_In_ EProjectionDirectionType eprjDir, _Out_ std::vector<Framework::CMultipleVariable>* pVctResult, _In_opt_ double dblAngle = 0.0, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 히스토그램 알고리즘을 합니다.
			*
			* param[out] pVctResult : 분석 결과
			* param[out_opt] ullPixelTotalCnt : 총 데이터 수
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Histogram(_Out_ std::vector<Framework::CMultipleVariable>* pVctResult, _Out_opt_ unsigned long long* ullPixelTotalCnt = nullptr, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 히스토그램 정규화 알고리즘을 합니다.
			*
			* param[out] pVctResult : 정규화된 분석 결과 (Pointer)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult HistogramNormalize(_Out_ std::vector<Framework::CMultipleVariable>* pVctResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 히스토그램 평활화 알고리즘을 합니다.
			*
			* param[out] pVctResult : 평활화된 분석 결과 (Pointer)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult HistogramEqualize(_Out_ std::vector<Framework::CMultipleVariable>* pVctResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지를 0에서 255 사이의 범위로 변환 알고리즘을 합니다.
			*
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Stretch(_In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지를 0에서 255 사이의 범위로 변환 알고리즘을 합니다.
			* 채널이 1개인 경우에 동작합니다.
			*
			* param[in_opt] rmvMinMax1 : 최소, 최대 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Stretch(_In_opt_ Framework::CMultipleVariable rmvMinMax1, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지를 0에서 255 사이의 범위로 변환 알고리즘을 합니다.
			* 채널이 2개인 경우에 동작합니다.
			*
			* param[in_opt] rmvMinMax1 : 채널1 최소, 최대 값
			* param[in_opt] rmvMinMax2 : 채널2 최소, 최대 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Stretch(_In_opt_ Framework::CMultipleVariable rmvMinMax1, _In_opt_ Framework::CMultipleVariable rmvMinMax2, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지를 0에서 255 사이의 범위로 변환 알고리즘을 합니다.
			* 채널이 3개인 경우에 동작합니다.
			*
			* param[in_opt] rmvMinMax1 : 채널1 최소, 최대 값
			* param[in_opt] rmvMinMax2 : 채널2 최소, 최대 값
			* param[in_opt] rmvMinMax3 : 채널3 최소, 최대 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Stretch(_In_opt_ Framework::CMultipleVariable rmvMinMax1, _In_opt_ Framework::CMultipleVariable rmvMinMax2, _In_opt_ Framework::CMultipleVariable rmvMinMax3, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지를 0에서 255 사이의 범위로 변환 알고리즘을 합니다.
			* 채널이 4개인 경우에 동작합니다.
			*
			* param[in_opt] rmvMinMax1 : 채널1 최소, 최대 값
			* param[in_opt] rmvMinMax2 : 채널2 최소, 최대 값
			* param[in_opt] rmvMinMax3 : 채널3 최소, 최대 값
			* param[in_opt] rmvMinMax4 : 채널4 최소, 최대 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Stretch(_In_opt_ Framework::CMultipleVariable rmvMinMax1, _In_opt_ Framework::CMultipleVariable rmvMinMax2, _In_opt_ Framework::CMultipleVariable rmvMinMax3, _In_opt_ Framework::CMultipleVariable rmvMinMax4, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에 이진화 알고리즘을 적용합니다.
			*
			* param[in] rmvtThreshold : 이진화 값
			* param[in_opt] rmvnCondition : 이진화 연산 방법
			* param[in_opt] rmvtRangeFillValue : 조건 내 설정 값
			* param[in_opt] rmvtOutOfRangeFillValue : 조건 외 설정 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			* param[in_opt] eLogicalOperator : 다중 채널의 논리연산 처리 방법
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SingleThreshold(_In_ Framework::CMultipleVariable rmvtThreshold, _In_opt_ Framework::CMultipleCondition rmvnCondition = ELogicalCondition_GreaterEqual, _In_opt_ Framework::CMultipleVariable rmvtRangeFillValue = EImageDepthRange_Max, _In_opt_ Framework::CMultipleVariable rmvtOutOfRangeFillValue = EImageDepthRange_Min, _In_opt_ CRavidGeometry* pGeometry = nullptr, _In_opt_ CRavidImage::EThresholdLogicalOperator eLogicalOperator = EThresholdLogicalOperator_None);
			
			/**
			* 입력 값으로 현재 이미지에 이중 범위 이진화 알고리즘을 적용합니다.
			*
			* param[in] rmvtLowThreshold : 이진화 하위 값
			* param[in] rmvtHighThreshold : 이진화 상위 값
			* param[in_opt] rmvtRangeFillValue : 조건 내 설정 값
			* param[in_opt] rmvtLesserThanLowFillValue : 하위 조건 외 설정 값
			* param[in_opt] rmvtGreaterThanHighFillValue : 상위 조건 외 설정 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult DoubleThreshold(_In_ Framework::CMultipleVariable rmvtLowThreshold, _In_ Framework::CMultipleVariable rmvtHighThreshold, _In_opt_ Framework::CMultipleVariable rmvtRangeFillValue = EImageDepthRange_Max, _In_opt_ Framework::CMultipleVariable rmvtLesserThanLowFillValue = EImageDepthRange_Min, _In_opt_ Framework::CMultipleVariable rmvtGreaterThanHighFillValue = EImageDepthRange_Min, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에 Otsu 알고리즘의 이진화 값을 계산합니다.
			*
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : CMultipleVariable 의 결과 값을 반환합니다.
			*/
			_Out_ Framework::CMultipleVariable GetOtsuThreshold(_In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에 Otsu 알고리즘을 적용합니다.
			*
			* param[in_opt] rmvnCondition : 이진화 연산 방법
			* param[in_opt] rmvtRangeFillValue : 조건 내 설정 값
			* param[in_opt] rmvtOutOfRangeFillValue : 조건 외 설정 값
			* param[out_opt] pOutThreshold : Otsu 알고리즘의 이진화 결과 값 (Pointer)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult OtsuThreshold(_In_opt_ Framework::CMultipleCondition rmvnCondition = ELogicalCondition_GreaterEqual, _In_opt_ Framework::CMultipleVariable rmvtRangeFillValue = EImageDepthRange_Max, _In_opt_ Framework::CMultipleVariable rmvtOutOfRangeFillValue = EImageDepthRange_Min, _Out_opt_ Framework::CMultipleVariable* pOutThreshold = nullptr, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에 적응적 이진화 알고리즘을 적용합니다.
			*
			* param[in_opt] eATT : 이진화 처리 시 반전 설정
			* param[in_opt] eATM : 이진화 처리 시 필터
			* param[in_opt] rmvMaxValue : 조건 내 설정 값
			* param[in_opt] nBlockSize : 처리 블럭 크기
			* param[in_opt] rmvConstant : 이진화 기준 상수 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult AdaptiveThreshold(_In_opt_ EAdaptiveThresholdType eATT = EAdaptiveThresholdType::EAdaptiveThresholdType_Binary, _In_opt_ EAdaptiveThresholdMethod eATM = EAdaptiveThresholdMethod::EAdaptiveThresholdMethod_Mean, _In_opt_ Framework::CMultipleVariable rmvMaxValue = 255, _In_opt_ int nBlockSize = 3, _In_opt_ Framework::CMultipleVariable rmvConstant = 10.0, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에 Isodata 이진화 알고리즘의 값을 계산합니다.
			*
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : CMultipleVariable 의 결과 값을 반환합니다.
			*/
			_Out_ Framework::CMultipleVariable GetIsodataThreshold(_In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에 Isodata 이진화 알고리즘을 적용합니다.
			*
			* param[in_opt] rmcCondition : 이진화 연산 방법
			* param[in_opt] rmvRangeFillValue : 조건 내 설정 값
			* param[in_opt] rmvOutOfRangeFillValue : 조건 외 설정 값
			* param[out_opt] pOutThreshold : 알고리즘의 이진화 결과 값 (Pointer)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult IsodataThreshold(_In_opt_ Framework::CMultipleCondition rmcCondition = ELogicalCondition_GreaterEqual, _In_opt_ Framework::CMultipleVariable rmvRangeFillValue = EImageDepthRange_Max, _In_opt_ Framework::CMultipleVariable rmvOutOfRangeFillValue = EImageDepthRange_Min, _Out_opt_ Framework::CMultipleVariable* pOutThreshold = nullptr, _In_opt_ CRavidGeometry *pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에 MaximumEntropy 이진화 알고리즘의 값을 계산합니다.
			*
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : CMultipleVariable 의 결과 값을 반환합니다.
			*/
			_Out_ Framework::CMultipleVariable GetMaximumEntropyThreshold(_In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에 MaximumEntropy 이진화 알고리즘을 적용합니다.
			*
			* param[in_opt] rmcCondition : 이진화 연산 방법
			* param[in_opt] rmvRangeFillValue : 조건 내 설정 값
			* param[in_opt] rmvOutOfRangeFillValue : 조건 외 설정 값
			* param[out_opt] pOutThreshold : 알고리즘의 이진화 결과 값 (Pointer)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult MaximumEntropyThreshold(_In_opt_ Framework::CMultipleCondition rmvnCondition = ELogicalCondition_GreaterEqual, _In_opt_ Framework::CMultipleVariable rmvtRangeFillValue = EImageDepthRange_Max, _In_opt_ Framework::CMultipleVariable rmvtOutOfRangeFillValue = EImageDepthRange_Min, _Out_opt_ Framework::CMultipleVariable* pOutThreshold = nullptr, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* FFT 변환 전 정보를 가져옵니다.
			*
			* return value : sFFTInfo* 의 FFT 변환 전 정보를 반환합니다.
			*/
			sFFTInfo* GetFFTInfo();
			
			/**
			* 입력 값으로 현재 이미지에 FFT 알고리즘을 적용합니다.
			*
			* param[in_opt] eFFTMethod : FFT 처리 방법
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult FFT(_In_opt_ EFFTMethod eFFTMethod = EFFTMethod_ForwardFloatShift, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에 Morphology 알고리즘을 적용합니다.
			*
			* param[in] nHalfWidth : 계산 가로 영역 (절반 크기)
			* param[in] nHalfHeight : 계산 세로 영역 (절반 크기)
			* param[in_opt] eMorphologyMethod : 모폴로지 연산 방법
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Morphology(_In_ int nHalfWidth, _In_ int nHalfHeight, _In_opt_ EMorphologyMethod eMorphologyMethod = EMorphologyMethod_Erode, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 최소값을 찾습니다.
			*
			* param[out] rmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMin(_Out_ Framework::CMultipleVariable& rmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 최소값을 찾습니다.
			*
			* param[out] pRmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMin(_Out_ Framework::CMultipleVariable* pRmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 최대값을 찾습니다.
			*
			* param[out] rmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMax(_Out_ Framework::CMultipleVariable& rmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 최대값을 찾습니다.
			*
			* param[out] pRmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMax(_Out_ Framework::CMultipleVariable* pRmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 최소, 최대값을 찾습니다.
			*
			* param[out] RmvMinResult : 최소 결과 값
			* param[out] RmvMaxResult : 최대 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMinMax(_Out_ Framework::CMultipleVariable& RmvMinResult, _Out_ Framework::CMultipleVariable& RmvMaxResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 최소, 최대값을 찾습니다.
			*
			* param[out] pRmvMinResult : 최소 결과 값
			* param[out] pRmvMaxResult : 최대 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMinMax(_Out_ Framework::CMultipleVariable* pRmvMinResult, _Out_ Framework::CMultipleVariable* pRmvMaxResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 평균값을 찾습니다.
			*
			* param[out] rmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMean(_Out_ Framework::CMultipleVariable& rmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 평균값을 찾습니다.
			*
			* param[out] pRmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMean(_Out_ Framework::CMultipleVariable* pRmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 합을 계산합니다.
			*
			* param[out] rmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetSum(_Out_ Framework::CMultipleVariable& rmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 합을 계산합니다.
			*
			* param[out] pRmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetSum(_Out_ Framework::CMultipleVariable* pRmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 중앙값을 찾습니다.
			*
			* param[out] rmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMedian(_Out_ Framework::CMultipleVariable& rmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 중앙값을 찾습니다.
			*
			* param[out] pRmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMedian(_Out_ Framework::CMultipleVariable* pRmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 분산을 계산합니다.
			*
			* param[out] rmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetVariance(_Out_ Framework::CMultipleVariable& rmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 분산을 계산합니다.
			*
			* param[out] pRmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetVariance(_Out_ Framework::CMultipleVariable* pRmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 표준편차를 계산합니다.
			*
			* param[out] rmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetStandardDeviation(_Out_ Framework::CMultipleVariable& rmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 표준편차를 계산합니다.
			*
			* param[out] pRmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetStandardDeviation(_Out_ Framework::CMultipleVariable* pRmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 공분산을 계산합니다.
			*
			* param[out] rmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetCovariance(_Out_ Framework::CMultipleVariable& rmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 공분산을 계산합니다.
			*
			* param[out] pRmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetCovariance(_Out_ Framework::CMultipleVariable* pRmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 상관계수를 계산합니다.
			*
			* param[out] rmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetCorrelationCoefficient(_Out_ Framework::CMultipleVariable& rmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 상관계수를 계산합니다.
			*
			* param[out] pRmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetCorrelationCoefficient(_Out_ Framework::CMultipleVariable* pRmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 합제곱을 계산합니다.
			*
			* param[out] rmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetSumSqr(_Out_ Framework::CMultipleVariable& rmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 합제곱을 계산합니다.
			*
			* param[out] pRmvResult : 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetSumSqr(_Out_ Framework::CMultipleVariable* pRmvResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 평균, 분산을 계산합니다.
			*
			* param[out] rmvMeanResult : 평균 결과 값
			* param[out] pRmvVarianceResult : 분산 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMeanVar(_Out_ Framework::CMultipleVariable& rmvMeanResult, _Out_ Framework::CMultipleVariable& pRmvVarianceResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 평균, 분산을 계산합니다.
			*
			* param[out] pRmvMeanResult : 평균 결과 값
			* param[out] pRmvVarianceResult : 분산 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMeanVar(_Out_ Framework::CMultipleVariable* pRmvMeanResult, _Out_ Framework::CMultipleVariable* pRmvVarianceResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 평균, 표준편차을 계산합니다.
			*
			* param[out] RmvMeanResult : 평균 결과 값
			* param[out] RmvStandardDeviationResult : 표준편차 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMeanStDev(_Out_ Framework::CMultipleVariable& RmvMeanResult, _Out_ Framework::CMultipleVariable& RmvStandardDeviationResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 평균, 표준편차을 계산합니다.
			*
			* param[out] pRmvMeanResult : 평균 결과 값
			* param[out] pRmvVarianceResult : 표준편차 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMeanStDev(_Out_ Framework::CMultipleVariable* pRmvMeanResult, _Out_ Framework::CMultipleVariable* pRmvStandardDeviationResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 평균, 분산, 표준편차을 계산합니다.
			*
			* param[out] rmvMeanResult : 평균 결과 값
			* param[out] rmvVarianceResult : 분산 결과 값
			* param[out] pRmvStandardDeviationResult : 표준편차 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMeanVarStDev(_Out_ Framework::CMultipleVariable& rmvMeanResult, _Out_ Framework::CMultipleVariable& rmvVarianceResult, _Out_ Framework::CMultipleVariable& pRmvStandardDeviationResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 평균, 분산, 표준편차을 계산합니다.
			*
			* param[out] pRmvMeanResult : 평균 결과 값
			* param[out] rmvVarianceResult : 분산 결과 값
			* param[out] pRmvStandardDeviationResult : 표준편차 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMeanVarStDev(_Out_ Framework::CMultipleVariable* pRmvMeanResult, _Out_ Framework::CMultipleVariable* rmvVarianceResult, _Out_ Framework::CMultipleVariable* pRmvStandardDeviationResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 분산, 표준편차을 계산합니다.
			*
			* param[out] RmvVarianceResult : 분산 결과 값
			* param[out] RmvStandardDeviationResult : 표준편차 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetVarStDev(_Out_ Framework::CMultipleVariable& RmvVarianceResult, _Out_ Framework::CMultipleVariable& RmvStandardDeviationResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 분산, 표준편차을 계산합니다.
			*
			* param[out] pRmvVarianceResult : 분산 결과 값
			* param[out] pRmvStandardDeviationResult : 표준편차 결과 값
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetVarStDev(_Out_ Framework::CMultipleVariable* pRmvVarianceResult, _Out_ Framework::CMultipleVariable* pRmvStandardDeviationResult, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 32 bit 정수 적분 이미지를 생성합니다.
			* 마지막 인자가 nullptr 이면 현재 이미지를 변형합니다.
			*
			* param[in_opt] dblQuadricCoefficient : 2차 계수
			* param[in_opt] dblLinear : 1차 계수
			* param[in_opt] dblConstant : 상수
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			* param[in_opt] pTargetImgInfo : 출력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult MakeIntegralTypeInt32(_In_opt_ double dblQuadricCoefficient = 0, _In_opt_ double dblLinear = 1, _In_opt_ double dblConstant = 0, _In_opt_ CRavidGeometry* pGeometry = nullptr, _In_opt_ Ravid::Algorithms::CRavidImage* pTargetImgInfo = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 64 bit 정수 적분 이미지를 생성합니다.
			* 마지막 인자가 nullptr 이면 현재 이미지를 변형합니다.
			*
			* param[in_opt] dblQuadricCoefficient : 2차 계수
			* param[in_opt] dblLinear : 1차 계수
			* param[in_opt] dblConstant : 상수
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			* param[in_opt] pTargetImgInfo : 출력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult MakeIntegralTypeInt64(_In_opt_ double dblQuadricCoefficient = 0, _In_opt_ double dblLinear = 1, _In_opt_ double dblConstant = 0, _In_opt_ CRavidGeometry* pGeometry = nullptr, _In_opt_ Ravid::Algorithms::CRavidImage* pTargetImgInfo = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 32 bit 소수 적분 이미지를 생성합니다.
			* 마지막 인자가 nullptr 이면 현재 이미지를 변형합니다.
			*
			* param[in_opt] dblQuadricCoefficient : 2차 계수
			* param[in_opt] dblLinear : 1차 계수
			* param[in_opt] dblConstant : 상수
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			* param[in_opt] pTargetImgInfo : 출력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult MakeIntegralTypeFloat(_In_opt_ double dblQuadricCoefficient = 0, _In_opt_ double dblLinear = 1, _In_opt_ double dblConstant = 0, _In_opt_ CRavidGeometry* pGeometry = nullptr, _In_opt_ Ravid::Algorithms::CRavidImage* pTargetImgInfo = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에서 64 bit 소수 적분 이미지를 생성합니다.
			* 마지막 인자가 nullptr 이면 현재 이미지를 변형합니다.
			*
			* param[in_opt] dblQuadricCoefficient : 2차 계수
			* param[in_opt] dblLinear : 1차 계수
			* param[in_opt] dblConstant : 상수
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			* param[in_opt] pTargetImgInfo : 출력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult MakeIntegralTypeDouble(_In_opt_ double dblQuadricCoefficient = 0, _In_opt_ double dblLinear = 1, _In_opt_ double dblConstant = 0, _In_opt_ CRavidGeometry* pGeometry = nullptr, _In_opt_ Ravid::Algorithms::CRavidImage* pTargetImgInfo = nullptr);
			
			/**
			* 현재 이미지에 정규화 알고리즘을 적용합니다.
			*
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Normalize(_In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 현재 이미지에 평활화 알고리즘을 적용합니다.
			*
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Equalize(_In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 현재 이미지에 적응적 평활화 알고리즘을 적용합니다.
			* 8 bit 1ch 만 가능합니다.
			*
			* param[in_opt] ui32DirectionX : 가로 방향 분할 수
			* param[in_opt] ui32DirectionY : 세로 방향 분할 수
			* param[in_opt] i32Min : 최소 데이터 값
			* param[in_opt] i32Max : 최대 데이터 값
			* param[in_opt] f32ClipLimit : 히스토그램 데이터 크기 제한
			* param[in_opt] ui32Bin : 히스토그램 범위 (1 ~ 256)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult AdaptiveEqualize(_In_opt_ uint32_t ui32DirectionX = 4, _In_opt_ uint32_t ui32DirectionY = 4, _In_opt_ int32_t i32Min = 0, _In_opt_ int32_t i32Max = 255, _In_opt_ float f32ClipLimit = 0.4, _In_opt_ uint32_t ui32Bin = 256, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 현재 이미지에 평활화 알고리즘을 적용합니다.
			*
			* param[in_opt] eDepth : 변환 할 픽셀 포맷 설정
			* param[in_opt] eValueMode : 변환 시 데이터 변환 설정
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertDepth(_In_ EConvertDepth eDepth, _In_opt_ EConvertDepthValueMode eValueMode = EConvertDepthValueMode_ChangeValue);
			
			/**
			* 현재 이미지에 반전된 데이터 값을 적용합니다.
			*
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult InvertValue(_In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 현재 이미지에서 모멘트를 계산합니다.
			*
			* param[in] p : 가로 차수
			* param[in] q : 세로 차수
			* param[out] r : 결과 값 (Pointer)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetGeometricMoments(_In_ unsigned int p, _In_ unsigned int q, _Out_ double* r, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 현재 이미지에서 모멘트 불변량을 계산합니다.
			*
			* param[out] vctMoments : 결과 값 리스트 (Reference)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetInvariantMoments(_Out_ std::vector<double>& vctMoments, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력된 데이터로 모멘트 불변량을 계산합니다.
			* 값에 대한 이해가 있을 경우 사용합니다.
			*
			* param[in] M00 : 이미지 전체 값 입력
			* param[in] M01 : 이미지 가로 1차 해석
			* param[in] M02 : 이미지 가로 2차 해석
			* param[in] M03 : 이미지 가로 3차 해석
			* param[in] M10 : 이미지 세로 1차 해석
			* param[in] M11 : 이미지 가로 1차 세로 1차 해석
			* param[in] M12 : 이미지 가로 2차 세로 1차 해석
			* param[in] M20 : 이미지 세로 2차 해석
			* param[in] M21 : 이미지 가로 1차 세로 2차 해석
			* param[in] M30 : 이미지 세로 3차 해석
			* param[out] vctMoments : 결과 값 리스트 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetInvariantMoments(_In_ double M00, _In_ double M01, _In_ double M02, _In_ double M03,_In_ double M10, _In_ double M11, _In_ double M12, _In_ double M20, _In_ double M21, _In_ double M30, _Out_ std::vector<double>& vctMoments);
			
			/**
			* 현재 이미지에서 Zernike 모멘트 불변량을 계산합니다.
			*
			* param[in] nOrder : 모멘트 차수 값 입력
			* param[out] vctMoments : 결과 값 리스트 (Reference)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetZernikeMoment( _In_ int nOrder, _Out_ std::vector<std::complex<double>>& vctMoments, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 현재 이미지에서 PseudoZernike 모멘트 불변량을 계산합니다.
			*
			* param[in] nOrder : 모멘트 차수 값 입력
			* param[out] vctMoments : 결과 값 리스트 (Reference)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetPseudoZernikeMoment(_In_ int nOrder, _Out_ std::vector<std::complex<double>>& vctMoments, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 현재 이미지가 이진화 이미지인지 확인합니다.
			*
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : bool 의 이진화 여부를 반환합니다.
			*/
			bool IsMonoImage(_In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 현재 이미지를 부호를 포함한 데이터로 변환합니다.
			*
			* param[in_opt] bSigned : 부호 설정
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetSigned(_In_opt_ bool bSigned = false);
			
			/**
			* 현재 이미지가 부호를 포함한 데이터인지 확인합니다.
			*
			* return value : bool 의 부호를 포함한 데이터 여부를 반환합니다.
			*/
			bool IsSigned();
			
			/**
			* YUV422 현재 이미지를 3채널 RGB 이미지로 변환합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ConvertYUV422ToRGB();
			
			/**
			* YUV422 현재 이미지를 3채널 RGB 이미지로 변환합니다.
			*
			* param[in_opt] pImgInfoSrcYUV422 : 입력 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CopyYUV422ToRGB(_In_ CRavidImage* pImgInfoSrcYUV422);
			
			/**
			* 현재 이미지에서 특정 채널만 추출하여 적용합니다.
			*
			* param[in_opt] nChannelNumber : 추출할 채널
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			* param[in_opt] mvFillValue : 빈 공간을 채울 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ExtractChannelComponent(_In_ int nChannelNumber, _In_opt_ CRavidGeometry* pGeometry = nullptr, _In_opt_ Framework::CMultipleVariable mvFillValue = 0);
	
			/**
			* 입력 값으로 현재 이미지에 MinimumEntropy 이진화 알고리즘의 값을 계산합니다.
			*
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : CMultipleVariable 의 결과 값을 반환합니다.
			*/
			_Out_ Framework::CMultipleVariable GetMinimumEntropyThreshold(_In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력 값으로 현재 이미지에 MinimumEntropy 이진화 알고리즘을 적용합니다.
			*
			* param[in_opt] rmcCondition : 이진화 연산 방법
			* param[in_opt] rmvRangeFillValue : 조건 내 설정 값
			* param[in_opt] rmvOutOfRangeFillValue : 조건 외 설정 값
			* param[out_opt] pOutThreshold : 알고리즘의 이진화 결과 값 (Pointer)
			* param[in_opt] pGeometry : 적용 영역 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult MinimumEntropyThreshold(_In_opt_ Framework::CMultipleCondition rmvnCondition = ELogicalCondition_GreaterEqual, _In_opt_ Framework::CMultipleVariable rmvtRangeFillValue = EImageDepthRange_Max, _In_opt_ Framework::CMultipleVariable rmvtOutOfRangeFillValue = EImageDepthRange_Min, _Out_opt_ Framework::CMultipleVariable* pOutThreshold = nullptr, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
		private:

			void Init();
			void Clean();

			bool SetFFTInfo(_In_ sFFTInfo* pFFTOrginInfo);

			int CalculatePixelSizeByte(_In_opt_ int nChannels = 0, _In_opt_ int nDepth = 0);
			int CalculateWidthStep(_In_ ptrdiff_t nSizeX, _In_ int nPixelSizeByte, _In_ int nAlignBytes);

			bool CreateOffsetTable();
			bool ConfigurateIplImage();

			CInternalRavidImage* m_pInternal = nullptr;
		};
	}
}