#pragma once

#include "RavidImageView.h"

#include <complex>

namespace Ravid
{
	namespace Framework
	{
		class CViewObject;
		class CViewObjectElement;
		class CRavidGraphViewPlotInfomation;

		class AFX_EXT_CLASS CRavidGraphView : public CRavidImageView
		{
			RavidUseDynamicCreation();

			enum ELeastSquareMode
			{
				ELeastSquareMode_None = 0,
				ELeastSquareMode_Linear,
				ELeastSquareMode_Quadratic,
				ELeastSquareMode_Cubic,
				ELeastSquareMode_Quartic,
				ELeastSquareMode_Quintic,
				ELeastSquareMode_Sextic,
				ELeastSquareMode_Septic,
				ELeastSquareMode_Octic,
				ELeastSquareMode_Nonic,
				ELeastSquareMode_Decic,
			};

		protected: // serialization에서만 만들어집니다.
			CRavidGraphView();
			CRavidGraphView(const CRavidGraphView& riv);
			DECLARE_DYNCREATE(CRavidGraphView)

		// 특성입니다.
		public:
			CRavidDoc* GetDocument() const;

		// 작업입니다.
		public:

		// 재정의입니다.
		public:
			virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
			virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


		// 구현입니다.
		public:
			virtual ~CRavidGraphView();
#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
#endif

		// 생성된 메시지 맵 함수
		protected:
			DECLARE_MESSAGE_MAP()

		public:
			virtual void OnInitialUpdate();
			afx_msg void OnDestroy();
			afx_msg void OnDropFiles(HDROP hDropInfo);
			afx_msg void OnSize(UINT nType, int cx, int cy);

		public:
			virtual bool DrawGraph();

			virtual void SetGraphTitle(_In_ CString strGraphTitle);
			CString GetGraphTitle();


			virtual int GetPlotInfoCount();

			virtual bool AddPlotInfo(_In_ CRavidGraphViewPlotInfomation* pInfo);
			virtual bool DeletePlotInfo(_In_ int nIndex);

			virtual bool ClearPlotInfo();


			virtual bool SetRange(_In_ double dblRangeStartX, _In_ double dblRangeEndX, _In_ double dblRangeStartY, _In_ double dblRangeEndY);
			virtual bool GetRange(_Out_ double& dblRangeStartX, _Out_ double& dblRangeEndX, _Out_ double& dblRangeStartY, _Out_ double& dblRangeEndY);


			virtual void SetXAxisUnit(_In_ CString strUnit);
			virtual CString GetXAxisUnit();

			virtual void SetYAxisUnit(_In_ CString strUnit);
			virtual CString GetYAxisUnit();


			virtual int GetIndicatorCount();

			virtual bool AddXAxisIndicator(_In_ double dblRangeX, _In_ CString strTitle, _In_opt_ COLORREF clrLine = LIGHTCYAN, _In_opt_ int nLineWidth = 1, _In_opt_ ERavidImageViewPenStyle eLinePenStyle = ERavidImageViewPenStyle_SOLID);
			virtual bool AddYAxisIndicator(_In_ double dblRangeY, _In_ CString strTitle, _In_opt_ COLORREF clrLine = LIGHTCYAN, _In_opt_ int nLineWidth = 1, _In_opt_ ERavidImageViewPenStyle eLinePenStyle = ERavidImageViewPenStyle_SOLID);

			virtual bool DeleteIndicator(_In_ int nIndex);

			virtual bool ClearIndicate();


			virtual int GetHighLightCount();

			virtual bool AddXAxisHighLight(_In_ double dblRangeStartX, _In_ double dblRangeEndX, _In_opt_ COLORREF clrBackground = RGB(212, 244, 250));
			virtual bool AddYAxisHighLight(_In_ double dblRangeStartY, _In_ double dblRangeEndY, _In_opt_ COLORREF clrBackground = RGB(206, 251, 201));
			virtual bool AddXYAxisHighLight(_In_ double dblRangeStartX, _In_ double dblRangeEndX, _In_ double dblRangeStartY, _In_ double dblRangeEndY, _In_opt_ COLORREF clrBackground = RGB(250, 244, 192));

			virtual bool DeleteHighLight(_In_ int nIndex);
			
			virtual bool ClearHighLight();


			virtual void ShowTitle(_In_ bool bShow);
			virtual bool IsShowTitle();

			virtual void ShowXAxisScale(_In_ bool bShow);
			virtual bool IsShowXAxisScale();

			virtual void ShowYAxisScale(_In_ bool bShow);
			virtual bool IsShowYAxisScale();

			virtual void ShowXAxisUnit(_In_ bool bShow);
			virtual bool IsShowXAxisUnit();

			virtual void ShowYAxisUnit(_In_ bool bShow);
			virtual bool IsShowYAxisUnit();

			virtual void ShowXAxisGrid(_In_ bool bShow);
			virtual bool IsShowXAxisGrid();

			virtual void ShowYAxisGrid(_In_ bool bShow);
			virtual bool IsShowYAxisGrid();

			virtual void ShowCrossHair(_In_ bool bShow);
			virtual bool IsShowCrossHair();

			virtual void ShowToolTip(_In_ bool bShow);
			virtual bool IsShowToolTip();

			virtual void ShowXAxisHighLight(_In_ bool bShow);
			virtual bool IsShowXAxisHighLight();

			virtual void ShowYAxisHighLight(_In_ bool bShow);
			virtual bool IsShowYAxisHighLight();

			virtual void ShowXYAxisHighLight(_In_ bool bShow);
			virtual bool IsShowXYAxisHighLight();

			virtual void ShowXAxisIndicator(_In_ bool bShow);
			virtual bool IsShowXAxisIndicator();

			virtual void ShowYAxisIndicator(_In_ bool bShow);
			virtual bool IsShowYAxisIndicator();

			virtual void ShowPlotPoint(_In_ bool bShow);
			virtual bool IsShowPlotPoint();

			virtual void ShowLine(_In_ bool bShow);
			virtual bool IsShowLine();

			virtual bool CaptureScreen(_Out_ Algorithms::CRavidImage* pImgInfo) override;

			virtual bool SetEquation(_In_ std::vector<double> vctCoef);

			virtual bool SetLeastSquaresMode(_In_ ELeastSquareMode eMode, _In_opt_ bool bRedraw = true);
			virtual ELeastSquareMode GetLeastSquaresMode();

			virtual bool AddLeastSquaresData(_In_ CRavidPoint<double> rp, _In_opt_ bool bRedraw = true);
			virtual bool RemoveLastLeastSquaresData(_In_opt_ bool bRedraw = true);
			virtual bool ClearLeastSquaresData(_In_opt_ bool bRedraw = true);




			virtual bool Open(_In_opt_ LPCTSTR lpszFileName = nullptr) override;
			virtual bool Save(_In_opt_ LPCTSTR lpszFileName = nullptr) override;
			virtual bool Close() override;

			virtual bool SetImageInfo(_In_ Algorithms::CRavidImage* pImgInfo, _In_opt_ bool bZoomFit = true) override;

			virtual void ShowPixelValue(_In_ bool bShow) override;
			
			virtual void ShowPixelSpectrum(_In_ bool bShow) override;
			
			virtual void ShowPixelGrid(_In_ bool bShow) override;
			
			virtual bool SetAccuracyX(double dblAccuracyX) override;
			
			virtual bool SetAccuracyY(double dblAccuracyY) override;
			


			// Only framework use

			virtual bool OnImageOpen() override;
			virtual bool OnImageSave() override;
			virtual bool OnImageClose() override;
			virtual bool OnInspect(_In_ int nIndex) override;
			virtual bool OnTeaching(_In_ int nIndex) override;
			virtual bool OnEditTeaching(_In_ int nIndex) override;
			virtual bool OnMeasure(_In_ ERavidMeasurementType eType) override;
			virtual bool OnToggleLayer(_In_ int nIndex) override;
			virtual bool OnAccuracySetting() override;

			virtual bool SetPrevImageInfo(_In_ Algorithms::CRavidImage* pImgInfo) override;

			virtual bool AddViewObject(_In_ CViewObject* pRSO, _In_opt_ bool bAutoShape = true, _Out_opt_ CViewObject** ppCreateRSO = nullptr) override;
			virtual bool DeleteViewObject(_In_ CViewObject* pRSO) override;
			virtual bool ClearViewObject(_In_opt_ bool bIgnoreImageToolObject = false) override;

			virtual bool SelectViewObjectElement(_In_ CViewObjectElement* pRSOE) override;

		protected:

			virtual bool RunPopupMenu(CPoint point) override;

			virtual double GetZoomFitScale() override;

			virtual bool DrawImage() override;
			virtual bool DrawPixelValue() override;
			virtual bool DrawGrid() override;
			virtual bool DrawStatus() override;


			virtual bool GetDrawAxisInfo(_Inout_ CRavidRect<double>& rrBase);

			virtual bool GetDecimalPlaceCount(_In_ double dblValue, _Out_ int& nIntegralCount, _Out_ int& nDecimalCount);

			virtual CString GetExtractText(_In_ CString strText, _In_ int nTextSize, _In_ int nLimitWidth);

			std::vector<double> GetDerivative(_In_ std::vector<double>& vctCoef);
			double GetValueY(_In_ std::vector<double>& vctCoef, _In_ double dblX);

			virtual bool DrawLeastSquares();

			virtual CString GetValueString(_In_ double dblValue);
			virtual CString GetValueString(_In_ std::complex<double> cpxValue);

		protected:
			enum EHighLightType
			{
				EHighLightType_X = 0,
				EHighLightType_Y,
				EHighLightType_XY
			};

			typedef struct sHighLightInfo
			{
				EHighLightType eType = EHighLightType_X;

				double dblRangeStartX = 0.;
				double dblRangeEndX = 0.;

				double dblRangeStartY = 0.;
				double dblRangeEndY = 0.;

				int nLineWidth = 1;

				COLORREF clrBackground = BLACK;
				COLORREF clrLine = ERavidImageViewLayerTransparencyColor;
			}
			SHighLightInfo;

			typedef struct sHighLightDrawInfo
			{
				CRavidRect<double> rrHighLight;

				int nLineWidth = 1;

				COLORREF clrBackground = BLACK;
				COLORREF clrLine = ERavidImageViewLayerTransparencyColor;
			}
			SHighLightDrawInfo;

			enum EUnit
			{
				EUnit_1 = 0,
				EUnit_2,
				EUnit_5,
				EUnit_Count
			};

			typedef struct sDrawAxisInfo
			{
				double dblStart = 0;
				double dblEnd = 0;

				int nIntervalCount = 0;
				int nIntegralCount = 0;
				int nDecimalCount = 0;

				double dblUnit = 0.;
				int nUnitIntegralCount = 0;
				int nUnitDecimalCount = 0;

				int nMaxIntegralCount = 0;
				int nMaxDecimalCount = 0;
			}
			SDrawAxisInfo;

			enum EIndicatorType
			{
				EIndicatorType_X = 0,
				EIndicatorType_Y,
			};

			typedef struct sIndicatorInfo
			{
				EIndicatorType eType = EIndicatorType_X;

				double dblRange = 0.;

				CString strTitle = _T("");

				COLORREF clr = LIGHTCYAN;
				int nLineWidth = 1;
				ERavidImageViewPenStyle eLinePenStyle = ERavidImageViewPenStyle_SOLID;
			}
			SIndicatorInfo;

			CString m_strGraphTitle = _T("");

			std::vector<CRavidGraphViewPlotInfomation*> m_vctPlotInfos;

			std::vector<SHighLightInfo> m_vctHighLightInfo;

			std::vector<SIndicatorInfo> m_vctIndicatorInfo;

			double m_dblRangeStartX = 0.;
			double m_dblRangeEndX = 1.;
			double m_dblRangeStartY = 0.;
			double m_dblRangeEndY = 1.;

			CString m_strXAxisUnit = _T("");
			CString m_strYAxisUnit = _T("");

			SDrawAxisInfo m_sDrawAxisInfoX;
			SDrawAxisInfo m_sDrawAxisInfoY;

			CRavidRect<double> m_rrGraph;

			int m_nDrawAreaOffset = 10;
			int m_nTextOffset = 5;
			int m_nTitleTextSize = 20;
			int m_nScaleTextSize = 12;
			int m_nUnitTextSize = 10;
			int m_nIndicatorTextSize = 10;

			double m_dblMinimumXAxis = 50.;
			double m_dblMinimumYAxis = 50.;

			bool m_bIsShowTitle = true;
			bool m_bIsShowScaleX = true;
			bool m_bIsShowScaleY = true;
			bool m_bIsShowUnitX = true;
			bool m_bIsShowUnitY = true;

			bool m_bIsShowGridX = true;
			bool m_bIsShowGridY = true;

			bool m_bIsShowCrossHair = true;
			bool m_bIsShowToolTip = true;
			bool m_bIsShowHighLightX = true;
			bool m_bIsShowHighLightY = true;
			bool m_bIsShowHighLightXY = true;
			bool m_bIsShowIndicatorX = true;
			bool m_bIsShowIndicatorY = true;
			bool m_bIsShowPlotPoint = true;
			bool m_bIsShowLine = true;

			bool m_bIsEquationMode = false;

			std::vector<double> m_vctEquationCoef;

			std::vector<std::complex<double> > m_vctEquationCpxResult;
			std::vector<double> m_vctEquationResult;

			CRavidGraphView::ELeastSquareMode m_eLeastSquaresMode = CRavidGraphView::ELeastSquareMode_None;

			std::vector<CRavidPoint<double> > m_vctLeastSquaresData;
			
		};
	}
}

#ifndef _DEBUG  // RavidGraphView.cpp의 디버그 버전
inline Ravid::Framework::CRavidDoc* Ravid::Framework::CRavidGraphView::GetDocument() const
   { return reinterpret_cast<Ravid::Framework::CRavidDoc*>(m_pDocument); }
#endif

