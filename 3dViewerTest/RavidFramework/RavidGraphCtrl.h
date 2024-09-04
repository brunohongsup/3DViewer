#pragma once

#include "RavidObjectEx.h"
#include "../RavidCore/RavidRect.h"
#include "../RavidCore/RavidPoint.h"
#include <map>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidGraphCtrl : public CStatic, public CRavidObjectEx
		{

		public:


		public:
			enum EDrawStyle
			{
				EDrawStyle_LINE = 0,
				EDrawStyle_BAR,
				EDrawStyle_DOT,
			};


			typedef struct SPlotsInfo
			{
				CString strPlotTitle;
				std::map<double, double> mapElement;
				size_t szBold = 1;
				EDrawStyle edrawstyle;
				COLORREF color;

			}SPlotsInfo;


		public:

			CRavidGraphCtrl();
			virtual ~CRavidGraphCtrl();

			virtual BOOL Create(RECT& rect, CWnd* parent, UINT nID,
								DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
			virtual BOOL DestroyWindow();

			virtual void SetRange(_In_ const double& dblRangeStartX, _In_ const double& dblRangeEndX, _In_ const double& dblRangeStartY, _In_ const double& dblRangeEndY);
			virtual void GetRange(_Out_ double& fRangeStartX, _Out_ double& fRangeEndX, _Out_ double& fRangeStartY, _Out_ double& fRangeEndY);

			virtual void SetGraphTitle(_In_ const CString& strTitle);
			virtual void GetGraphTitle(_Out_ CString& strTitle);

			virtual void ShowGraphTitle(_In_ bool bShowTitle = true);
			virtual bool IsShowGraphTitle();

			virtual void SetXAxisTitle(_In_ const CString& strTitle);
			virtual void SetYAxisTitle(_In_ const CString& strTitle);

			virtual void GetXAxisTitle(_Out_ CString& strTitle);
			virtual void GetYAxisTitle(_Out_ CString& strTitle);

			virtual void ShowXAxisTitle(_In_ bool bShowTitle = true);
			virtual bool IsShowXAxisTitle();

			virtual void ShowYAxisTitle(_In_ bool bShowTitle = true);
			virtual bool IsShowYAxisTitle();

			virtual void ShowGrid(_In_ bool bShowGrid = true);
			virtual bool IsShowGrid();

			virtual bool AddLegend(_In_ const CString& PlotTitle);
			virtual void ShowLegend(_In_ bool bShowLegend = true);
			virtual bool IsShowLegend();
			virtual size_t GetLegendCount();

			virtual void ShowPixedValue(_In_ bool bShowValue = true);
			virtual bool IsShowPixedValue();

			virtual void ShowPixedPoint(_In_ bool bShowPoint = true);
			virtual bool IsShowPixedPoint();

			virtual void AutoScaleMode(_In_opt_ bool bAutoScaleEnable = true);

			virtual void AddPlotsInfo(_In_ const SPlotsInfo& splot);
			virtual void AddPlotElement(_In_ const CString& strPlotTitle, _In_ const std::pair<double, double> pairElement);
			virtual bool DeletePlotsInfo(_In_ size_t szindex);
			virtual void ClearPlotsInfo();
			virtual size_t GetPlotsCount();

			virtual void SetGraphStandardSize(_In_ CSize szSize);
			virtual CSize GetGraphStandardSize();

		public:
			afx_msg void OnPaint();
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
			afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
			afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
			afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
			afx_msg void OnSize(UINT nType, int cx, int cy);
		public:
			DECLARE_MESSAGE_MAP()

		protected:
			CCriticalSection m_cs;
			void EnableZoomX(bool bZoomX = true);
			void EnableZoomY(bool bZoomY = true);

		private:
			bool IsXAbleToZoom(bool bAbsolute = false);
			bool IsYAbleToZoom(bool bAbsolute = false);

			void __InternalSetXYMinMaxValue();
			void __InternalInitScale();
			void __InternalZoomXAllFit();
			void __InternalZoomYAllFit();
			void __InternalConvertLegendRegionValidString(CString& strLegend);
			void __InternalSetManualDataX(const double& dblXMin, const double& dblXMax);
			void __InternalSetManualDataY(const double& dblYMin, const double& dlbYMax);
			void __InternalSetDataX(const double& dblXMin, const double& dblXMax);
			void __InternalSetDataY(const double& dblYMin, const double& dlbYMax);
			void __InternalSetScreenDataX(const double& dblXMin, const double& dblXMax);
			void __InternalSetScreenDataY(const double& dblYMin, const double& dlbYMax);
			void __InternalRectInRectFit(const CRect& rc, CRavidRect<double>& rp);
			void __InternalRectRateToRect(const CRect& rc, CRavidRect<double>& rc2);
			void __InternalInitShowXY();
			void __InternalCompareShowXY(const double& nX, const double& nY);
			void __InternalCompareFrameXY(const double& nX, const double& nY);
			bool __InternalValidElement();
			bool __InternalCheckValueLimit(const double & dblValue);
			int __InternalRunPopupmenu(CPoint pt);
			size_t __InternalGetStringMaxLength(const CDC& dc, const double& dblValue1, const double& dblValue2);
			size_t __InternalGetStringYMaxLength(const CDC& dc, const double& dblValue1, const double& dblValue2);
			CRavidPoint<double> __InternalCalculationRectInRectRate(CRect rc, CRavidRect<double> rp2);
			CSize __InternalGetLegendMaxTextSize(const CDC& dc);
			long long __InternalRounds(const double & dblValue);
			int __InternalGetDecimalPlace(const double& dblValue);
			CString __InternalGetDecimalPlaceString(const int nValue);
			void __InternalAddPlotElement();
			void __InternalCalculationIncreasment();
			double __InternalPixelToGraphX(double dblPixel);
			int __InternalGraphToPixelX(double dblGraph);
			double __InternalPixelToGraphY(double dblPixel);
			int __InternalGraphToPixelY(double dblGraph);
			void __InternalSearchShowXY();

			enum EPopupmanue
			{

				EPopupMenu_Scale = 0,
				EPopupMenu_MOUSEWHEEL_ZOOMX,
				EPopupMenu_MOUSEWHEEL_ZOOMY,

				EPopupMenu_FIT,

				EPopupMenu_FIT_ALL,
				EPopupMenu_FIT_ALL_XY,
				EPopupMenu_FIT_ALL_X,
				EPopupMenu_FIT_ALL_Y,

				EPopupMenu_FIT_Screen,
				EPopupMenu_FIT_Screen_XY,
				EPopupMenu_FIT_Screen_X,
				EPopupMenu_FIT_Screen_Y,

				EPopupMenu_SET,
				EPopupMenu_SET_X,
				EPopupMenu_SET_Y,

				EPopupMenu_SHOW,

				EPopupMenu_SHOW_GRID,
				EPopupMenu_SHOW_GRID_X,
				EPopupMenu_SHOW_GRID_Y,

				EPopupMenu_SHOW_SCALE,
				EPopupMenu_SHOW_SCALE_X,
				EPopupMenu_SHOW_SCALE_Y,

				EPopupMenu_SHOW_LEGEND,

				EPopupMenu_SHOW_CROSSHAIR,
				EPopupMenu_SHOW_CROSSHAIRLOCATE,
				EPopupMenu_SHOW_CROSSHAIRVALUE,


				EPopupMenu_SHOW_PIXEDPOINT,
				EPopupMenu_SHOW_PIXEDVALUE,

				EPopupMenu_SHOW_XAXISTITLE,
				EPopupMenu_SHOW_YAXISTITLE,

				EPopupMenu_SHOW_GRAPHTITLE,

				EPopupMenu_ADDPLOT,

			};

			CString m_strTitle;
			CString m_XUnitTitle;
			CString m_YUnitTitle;

			bool m_bZoomX = true;
			bool m_bZoomY = true;

			double m_dblXMax = -DBL_MAX;
			double m_dblXMin = DBL_MAX;
			double m_dblYMax = -DBL_MAX;
			double m_dblYMin = DBL_MAX;

			//For Show Graph Data Range
			double m_dblShowXMax = -DBL_MAX;
			double m_dblShowXMin = DBL_MAX;
			double m_dblShowYMax = -DBL_MAX;
			double m_dblShowYMin = DBL_MAX;

			double m_dblShowXMaxPixel = -DBL_MAX;
			double m_dblShowXMinPixel = DBL_MAX;
			double m_dblShowYMaxPixel = -DBL_MAX;
			double m_dblShowYMinPixel = DBL_MAX;
			//For Save Graph Auto Data Range
			double m_dblAutoScaleXMax = -DBL_MAX;
			double m_dblAutoScaleXMin = DBL_MAX;
			double m_dblAutoScaleYMax = -DBL_MAX;
			double m_dblAutoScaleYMin = DBL_MAX;

			//For Save Graph Manual Data Range
			double m_dblManualScaleXMax = 0;
			double m_dblManualScaleXMin = 0;
			double m_dblManualScaleYMax = 0;
			double m_dblManualScaleYMin = 0;

			//For Show Graph Range
			double m_dblGraphXMin = 0;
			double m_dblGraphXMax = 0;
			double m_dblGraphYMin = 0;
			double m_dblGraphYMax = 0;

			//For Padding Value
			double m_dblGridYMax = -DBL_MAX;
			double m_dblGridYMin = DBL_MAX;
			double m_dblPreGridYMax = 0;
			double m_dblPreGridYMin = 0;
			double m_dblYUnitIncreasement = 0;
			double m_dblXUnitIncreasement = 0;

			//For Padding to DrawGraphAxis
			double m_dblPaddingX = 0.;
			double m_dblPaddingY = 0.;

			//For Scale 
			double m_dblOffsetX = 0.;
			double m_dblOffsetY = 0.;
			double m_dblXScale = 1.;
			double m_dblYScale = 1.;
			int m_nPreYUnitMaxLength = 0;
			int m_nOffsetSumX = 0;

			double m_dblXIncreasement = 0.;
			double m_dblYIncreasement = 0.;
			std::vector<SPlotsInfo> m_vctPlots;
			std::vector<SPlotsInfo> m_vctLegend;

			size_t m_szLegendMaxLength = 0;

			bool m_bAutoScaleMode = true;
			bool m_bShowGridX = true;
			bool m_bShowGridY = true;
			bool m_bShowScaleX = true;
			bool m_bShowScaleY = true;
			bool m_bShowCrossHair = true;
			bool m_bShowCrossHairLocate = true;
			bool m_bShowValue = true;
			bool m_bPaint = false;
			bool m_bFirstPaint = false;
			bool m_bSecondPaint = false;
			bool m_bShowLegend = true;
			bool m_bMouseClick = false;
			bool m_bMouseMClick = false;
			bool m_bOnSize = false;
			bool m_bFloatingPoint = false;
			bool m_bShowGraphTitle = true;
			bool m_bShowXAxisTitle = true;
			bool m_bShowYAxisTitle = true;
			bool m_bShowPixedValue = false;
			bool m_bShowPixedPoint = false;

			bool m_bZeroRangeX = false;
			bool m_bZeroRangeY = false;

			CRect m_crPreGraphControlLocate;				//컨트롤의 크기
			CRavidRect<double> m_rrPreGraphLocate;			//컨트롤안에 그려질 그래프의 크기
			CRavidRect<double> m_rrLegend;					//legend창의 크기
			CRavidRect<double> m_rrBaseGraphRegion;			//legend창의 크기


			CRavidPoint<double> m_rpGraphInLegendLocationRate = CPoint(0, 0);
			CPoint m_ptGraphLocate = CPoint(0, 0);
			CPoint m_ptLegendOffset = CPoint(0, 0);
			CPoint m_ptLegendResizeOffset = CPoint(0, 0);
			CPoint m_ptCurrent = CPoint(0, 0);

			CSize m_szStandardSize = CSize(383, 431);
		};
	}
}

