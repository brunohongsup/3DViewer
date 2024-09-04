#pragma once

namespace Ravid
{
	namespace Mathematics
	{
		class CMatrix;
	}

	namespace Algorithms
	{
		/**
		* 컨볼루션에서 사용하기 위한 프로세싱된 결과를 제공하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CConvolutionPreset : public CRavidObject
		{
		public:
			/**
			* 커널 정보입니다.
			*/
			enum EKernelType
			{
				EKernelType_Uniform3x3 = 0,	//3 by 3 고정
				EKernelType_Uniform5x5,		//5 by 5 고정
				EKernelType_Uniform7x7,		//7 by 7 고정
				EKernelType_UniformNxN,		//N by N 가변
				
				EKernelType_Gaussian3x3,	//3 by 3 고정
				EKernelType_Gaussian5x5,	//5 by 5 고정
				EKernelType_Gaussian7x7,	//7 by 7 고정
				EKernelType_GaussianNxN,	//N by N 가변

				EKernelType_Sharpen,		//3 by 3 고정

				EKernelType_LowPass1,		//3 by 3 고정
				EKernelType_LowPass2,		//3 by 3 고정
				EKernelType_LowPass3,		//3 by 3 고정

				EKernelType_HighPass1,		//3 by 3 고정
				EKernelType_HighPass2,		//3 by 3 고정

				EKernelType_GradientX,		//3 by 3 고정
				EKernelType_GradientY,		//3 by 3 고정

				EKernelType_SobelX,			//3 by 3 고정
				EKernelType_SobelY,			//3 by 3 고정

				EKernelType_PrewittX,		//3 by 3 고정
				EKernelType_PrewittY,		//3 by 3 고정

				EKernelType_LaplacianX,		//3 by 3 고정
				EKernelType_LaplacianY,		//3 by 3 고정

				EKernelType_Laplacian4,		//3 by 3 고정
				EKernelType_Laplacian8,		//3 by 3 고정

				EKernelType_CompassN,		//3 by 3 고정
				EKernelType_CompassNW,		//3 by 3 고정
				EKernelType_CompassW,		//3 by 3 고정
				EKernelType_CompassSW,		//3 by 3 고정
				EKernelType_CompassS,		//3 by 3 고정
				EKernelType_CompassSE,		//3 by 3 고정
				EKernelType_CompassE,		//3 by 3 고정
				EKernelType_CompassNE,		//3 by 3 고정
			};

			CConvolutionPreset();
			virtual ~CConvolutionPreset();
			
			/**
			* 입력된 커널 정보를 사용하여 메트릭스를 계산합니다.
			*
			* param[in] eKT : EKernelType enum 으로 연산 방법 및 값 분포 방법을 결정합니다. 커널 정보를 참고하세요.
			* param[out] m : CMatrix 의 형태로 입력 커널에 대한 메트릭스 값을 가져옵니다. (Pointer)
			* param[in_opt] nRow : 커널의 행 값 (가로)
			* param[in_opt] nCol : 커널의 열 값 (세로)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetKernel(_In_ EKernelType eKT, _Out_ Ravid::Mathematics::CMatrix& m, _In_opt_ int nRow = 3, _In_opt_ int nCol = 3);
		};
	}
}

