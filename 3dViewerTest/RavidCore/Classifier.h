#pragma once

#include <vector>

namespace Ravid
{
	namespace Mathematics
	{
		//using SVType = std::vector<double>;
		struct SParameter
		{
			int nKernelType;
			double dblGamma;
			double dblEps;
			double dblC;
			//double dblScale;

			std::vector<double> vctWeighted;
			std::vector<double> vctWeightedLabel;	

			uint32_t nMaxIteration;
			bool	bProbability;

			bool	bNormalizedByColumn;			
		};

		struct SModel
		{
			SParameter sParam;
			int	nClass;
			int nTotalSV;
			int nSizeFeature;

			std::vector<double>	vctRho;
			std::vector<double>	vctLabel;

			std::vector<std::vector<std::vector<double>>> vctSVs;
			std::vector<std::vector<double>>  vctSVCoefs;	

			std::vector<double> vctProbA;
			std::vector<double> vctProbB;

			//std::vector<double> vctNormalizedFactor;

			std::vector<double> vctFeatureMean;
			std::vector<double> vctFeatureStdDev;
		};

		struct SDecisionFunc
		{
			std::vector<double> vctAlpha;
			double  dblRho;
		};

		struct SConfusionMatrix
		{
			std::vector<double> vctTPR; // true positive rate
			std::vector<double> vctTNR; // true negative rate
			std::vector<double> vctPPV; // positive predictive value
			std::vector<double> vctNPV; // negative predict value
			std::vector<double> vctFPR; // false positive rate
			std::vector<double> vctFDR; // false discovery rate
			std::vector<double> vctFOR; // false omission rate

			double dblACC; // accuracy
			double dblF1;  // F1 score
			double dblMCC; // matthews correlation coefficient
			double dblBM;  // bookmaker informedness
			double dblMK;  // markedness

			std::vector<std::vector<ptrdiff_t>> matrix;
		};

		class IMLClassifier
		{
		public:
			virtual ~IMLClassifier() {};

			virtual bool Train(_In_ double** pData, _In_ double* pLabel, _In_ ptrdiff_t nSizeFeature, _In_ ptrdiff_t nSizeSample, _Out_ SModel& sModel) = 0;
			virtual bool Classify(_In_ const SModel& sModel, _In_ double* pData, _In_ ptrdiff_t nSizeFeature, _Out_ double& dblLabel, _Out_ double* pDecValues = nullptr) = 0;
			virtual bool ClassifyWithProbability(_In_ const SModel& sModel, _In_ double* pData, _In_ ptrdiff_t nSizeFeature, _Out_ double& dblLabel, _Out_ double& dblProbability) = 0;
			virtual bool ClassifyWithProbability(_In_ const SModel& sModel, _In_ double* pData, _In_ ptrdiff_t nSizeFeature, _Out_ std::vector<double>& vctLabel, _Out_ std::vector<double>& vctProbability) = 0;
			virtual bool Save(_In_ const SModel& sModel, _In_ const std::wstring& strFilePath) = 0;
			virtual bool Load(_Out_ SModel& sModel, _In_ const std::wstring& strFilePath) = 0;
			virtual bool GetConfusionMatrix(_In_ double* pLabel, _In_ double* pPred, _In_ double* pAct, _In_ ptrdiff_t nSizeLabel, _In_ ptrdiff_t nSizeSampling, _Out_ SConfusionMatrix& sMatrix) = 0;
			
		};
		
		/**
		* 서포트 벡터 머신(SVM) 분류 클래스입니다.
		* 현재 개발중입니다.
		*/
		class AFX_EXT_CLASS CSVMClassifier : public IMLClassifier, public CRavidObject
		{
		public:
			RavidPreventCopySelf(CSVMClassifier);

			CSVMClassifier();
			CSVMClassifier(const SParameter& sParam);
			virtual ~CSVMClassifier();

			/**
			* 동작 시간을 측정하는 클래스입니다.
			*/
			enum ESVMKernel
			{
				ESVMKernel_Linear = 0,
				ESVMKernel_RBF,
				ESVMKernel_Polynomial,
				ESVMKernel_Sigmoid,
			};

			void SetParameter(const SParameter& sParam);
			SParameter& GetParameter();	

			bool Train(_In_ double** pData, _In_ double* pLabel, _In_ ptrdiff_t nSizeFeature, _In_ ptrdiff_t nSizeSample, _Out_ SModel& sModel);
			bool Classify(_In_ const SModel& sModel, _In_ double* pData, _In_ ptrdiff_t nSizeFeature, _Out_ double& dblLabel, _Out_ double* pDecValues = nullptr);
			bool ClassifyWithProbability(_In_ const SModel& sModel, _In_ double* pData, _In_ ptrdiff_t nSizeFeature, _Out_ double& dblLabel, _Out_ double& dblProbability);
			bool ClassifyWithProbability(_In_ const SModel& sModel, _In_ double* pData, _In_ ptrdiff_t nSizeFeature, _Out_ std::vector<double>& vctLabel, _Out_ std::vector<double>& vctProbability);
			bool Save(_In_ const SModel& sModel, _In_ const std::wstring& strFilePath);
			bool Load(_Out_ SModel& sModel, _In_ const std::wstring& strFilePath);
		
			bool GetConfusionMatrix(_In_ double* pLabel, _In_ double* pPred, _In_ double* pAct, _In_ ptrdiff_t nSizeLabel, _In_ ptrdiff_t nSizeSampling, _Out_ SConfusionMatrix& sMatrix);
		
		private:

			SParameter m_sParam;
		};
	}
}


