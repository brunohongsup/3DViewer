#pragma once

#include <memory>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidGraphViewPlotInfomation
		{
		public:
			
			CRavidGraphViewPlotInfomation();
			CRavidGraphViewPlotInfomation(CRavidGraphViewPlotInfomation& cInfo);
			CRavidGraphViewPlotInfomation(CRavidGraphViewPlotInfomation* pInfo);
			virtual ~CRavidGraphViewPlotInfomation();

		public:

			virtual bool AddPlot(_In_ double dblX, _In_ double dblY);
			virtual bool ClearPlot();

			virtual size_t GetPlotCount();

			virtual CRavidGeometryArray* GetPlot();

			virtual void SetDotColor(_In_ COLORREF clr);
			virtual COLORREF GetDotColor();

			virtual void SetDotSize(_In_ int nSize);
			virtual int GetDotSize();

			virtual void SetLineColor(_In_ COLORREF clr);
			virtual COLORREF GetLineColor();

			virtual void SetLineWidth(_In_ int nWidth);
			virtual int GetLineWidth();

			virtual void SetPlotType(_In_ ERavidGraphViewPlotType eType);
			ERavidGraphViewPlotType GetPlotType();

		protected:
			CRavidGeometryArray* m_pRGAPlot = nullptr;
			COLORREF m_clrDot = RGB(0, 0, 0);
			COLORREF m_clrLine = RGB(0, 0, 0);
			int m_nDotSize = 1;
			int m_nLineWidth = 1;
			ERavidGraphViewPlotType m_ePlotType = ERavidGraphViewPlotType_Dot;

		};
	}
}

