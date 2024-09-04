#pragma once

#include <vector>

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
		class CInternalGridCalibration;

		class AFX_EXT_CLASS CGridCalibration
		{
		public:
			CGridCalibration();
			~CGridCalibration();

			RavidPreventCopySelf(CGridCalibration);

			/**
			* ������ mm �� �׸��� ����Ʈ�� �����Դϴ�.
			*/
			void GetRealIntervalX(double& reff64Acc);
			void GetRealIntervalY(double& reff64Acc);

			void SetRealIntervalX(double f64Acc);
			void SetRealIntervalY(double f64Acc);
	
			/**
			* �׸��� ����, ���� ���� �����մϴ�.
			*/
			void GetRegionCount(int32_t& refi32WidthCount, int32_t& refi32HeightCount);
			void SetRegionCount(int32_t i32WidthCount, int32_t i32HeightCount, bool bAlloc = true);

			/**
			* �ش� �ε����� ROI ���翩�θ� �����ɴϴ�.
			*/
			bool IsDataValid(int32_t w, int32_t h);
	
			/**
			* �ش� �ε����� ���� ��ǥ�� �Է��մϴ�.
			*/
			bool UpdateROI(int32_t w, int32_t h, CRavidPoint<double>& refSourcePoint);
	
			/**
			* �ش� �ε����� ROI �� �����մϴ�.
			*/
			bool ClearROI(int32_t w, int32_t h);
	
			/**
			* ���� ��ǥ�� ��ǥ ��ǥ�� ����մϴ�.
			*/
			bool SourceToTarget(CRavidPoint<double>& refSourcePoint, CRavidPoint<double>& refTargetPoint);
	
			/**
			* ��ǥ ��ǥ�� ���� ��ǥ�� ����մϴ�.
			*/
			bool TargetToSource(CRavidPoint<double>& refTargetPoint, CRavidPoint<double>& refSourcePoint);
	
			/**
			* ��ǥ ��ǥ�� ���� ��ǥ�� ����մϴ�.
			*/
			bool ConvertImage(CRavidImage* pInput, CRavidImage* pOutput);

			/**
			* ������ ���Ϸ� ����, �б�
			*/
			bool Save(CString strFile);
			bool Load(CString strFile);
	
			/**
			* �ʱ�ȭ ���¸� �о�ɴϴ�.
			*/
			bool IsAllocate();
		
			/**
			* �Էµ� ������ �о�ɴϴ�.
			*/
			std::vector<std::vector<CRavidPoint<double>*>>* GetImageROIS();

		private:
			CInternalGridCalibration* m_pInternal;
		};
	}
}


