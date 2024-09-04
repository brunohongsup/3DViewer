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
			* 단위는 mm 로 그리드 포인트간 간격입니다.
			*/
			void GetRealIntervalX(double& reff64Acc);
			void GetRealIntervalY(double& reff64Acc);

			void SetRealIntervalX(double f64Acc);
			void SetRealIntervalY(double f64Acc);
	
			/**
			* 그리드 가로, 세로 수를 정의합니다.
			*/
			void GetRegionCount(int32_t& refi32WidthCount, int32_t& refi32HeightCount);
			void SetRegionCount(int32_t i32WidthCount, int32_t i32HeightCount, bool bAlloc = true);

			/**
			* 해당 인덱스의 ROI 존재여부를 가져옵니다.
			*/
			bool IsDataValid(int32_t w, int32_t h);
	
			/**
			* 해당 인덱스에 원본 좌표를 입력합니다.
			*/
			bool UpdateROI(int32_t w, int32_t h, CRavidPoint<double>& refSourcePoint);
	
			/**
			* 해당 인덱스의 ROI 를 해제합니다.
			*/
			bool ClearROI(int32_t w, int32_t h);
	
			/**
			* 원본 좌표를 목표 좌표로 계산합니다.
			*/
			bool SourceToTarget(CRavidPoint<double>& refSourcePoint, CRavidPoint<double>& refTargetPoint);
	
			/**
			* 목표 좌표를 원본 좌표로 계산합니다.
			*/
			bool TargetToSource(CRavidPoint<double>& refTargetPoint, CRavidPoint<double>& refSourcePoint);
	
			/**
			* 목표 좌표를 원본 좌표로 계산합니다.
			*/
			bool ConvertImage(CRavidImage* pInput, CRavidImage* pOutput);

			/**
			* 정보를 파일로 저장, 읽기
			*/
			bool Save(CString strFile);
			bool Load(CString strFile);
	
			/**
			* 초기화 상태를 읽어옵니다.
			*/
			bool IsAllocate();
		
			/**
			* 입력된 영역을 읽어옵니다.
			*/
			std::vector<std::vector<CRavidPoint<double>*>>* GetImageROIS();

		private:
			CInternalGridCalibration* m_pInternal;
		};
	}
}


