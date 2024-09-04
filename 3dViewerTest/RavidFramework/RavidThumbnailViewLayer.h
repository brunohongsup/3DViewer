#pragma once

#include "RavidObjectEx.h"

#include <vector>
#include <map>

namespace Ravid
{
	namespace Framework
	{
		class CRavidThumbnailViewLayerElement;
		class CRavidImageViewLayer;

		class AFX_EXT_CLASS CRavidThumbnailViewLayer : public CRavidObjectEx
		{
			RavidUseDynamicCreation();

		public:
			CRavidThumbnailViewLayer(CRavidObjectEx* pParentView = nullptr);

			virtual ~CRavidThumbnailViewLayer();

		public:
			void Clear();

			bool DrawShape(_In_ int nIndex, _In_ CRavidGeometry& rgObject, _In_opt_ COLORREF crLineColor = 0, _In_opt_ long nLineWidth = 1, _In_opt_ COLORREF crFillColor = ERavidImageViewLayerTransparencyColor, _In_opt_ ERavidImageViewPenStyle ePenStyle = ERavidImageViewPenStyle_SOLID, _In_opt_ float fLineColorAlphaRatio = 1.f, _In_opt_ float fFillColorAlphaRatio = 1.f);
			bool DrawShape(_In_ int nIndex, _In_ CRavidGeometry* pRgObject, _In_opt_ COLORREF crLineColor = 0, _In_opt_ long nLineWidth = 1, _In_opt_ COLORREF crFillColor = ERavidImageViewLayerTransparencyColor, _In_opt_ ERavidImageViewPenStyle ePenStyle = ERavidImageViewPenStyle_SOLID, _In_opt_ float fLineColorAlphaRatio = 1.f, _In_opt_ float fFillColorAlphaRatio = 1.f);
			bool DrawShape(_In_ int nIndex, _In_ SRavidDrawingStruct* pShapeElement);

			bool DrawText(_In_ int nIndex, _In_ CRavidGeometry& rgObject, _In_ CString strText, _In_opt_ COLORREF crTextColor = 0, _In_opt_ COLORREF crFillColor = ERavidImageViewLayerTransparencyColor, _In_opt_ long nFontSize = 12, _In_opt_ bool bRealSize = false, _In_opt_ double dblAngle = 0., _In_opt_ DWORD dwAlign = TA_LEFT | TA_TOP, _In_opt_ CString* pFontName = nullptr, _In_opt_ float fTextColorAlphaRatio = 1.f, _In_opt_ float fFillColorAlphaRatio = 1.f);
			bool DrawText(_In_ int nIndex, _In_ CRavidGeometry* pRgObject, _In_ CString strText, _In_opt_ COLORREF crTextColor = 0, _In_opt_ COLORREF crFillColor = ERavidImageViewLayerTransparencyColor, _In_opt_ long nFontSize = 12, _In_opt_ bool bRealSize = false, _In_opt_ double dblAngle = 0., _In_opt_ DWORD dwAlign = TA_LEFT | TA_TOP, _In_opt_ CString* pFontName = nullptr, _In_opt_ float fTextColorAlphaRatio = 1.f, _In_opt_ float fFillColorAlphaRatio = 1.f);

			bool GetLayerShapeElement(_In_ int nIndex, _Out_ std::vector<SRavidDrawingStruct>* pVctLayerShapeElement);
			bool GetLayerTextElement(_In_ int nIndex, _Out_ std::vector<CRavidThumbnailViewLayerElement*>* pVctLayerTextElement);

			bool DeleteLayerElement(_In_ int nIndex);

			bool Copy(_In_ int nIndex, _In_ CRavidImageViewLayer* pSrcLayer);

		protected:
			bool __InternalDrawShapeCDC(_In_ int nIndex, _In_ CRavidGeometry* pRgObject, _In_ COLORREF crLineColor, _In_ long nLineWidth, _In_ COLORREF crFillColor, _In_ ERavidImageViewPenStyle ePenStyle, _In_ float fLineColorAlphaRatio, _In_ float fFillColorAlphaRatio, _In_ bool bIsExclusive, _In_ long long llShapeIndex);

			bool InternalDraw(_In_ CDC* pDC, _In_ ID2D1RenderTarget* pRender, _In_ CRavidPoint<double> frpOffset, _In_ double fScale, _In_ CRavidRect<int> rreWindowRect);

			bool InternalDrawText(_In_ Algorithms::CRavidImage* pImage, _In_ CDC* pDC, _In_ ID2D1RenderTarget* pRender, _In_ CRavidThumbnailViewLayerElement* pRIVLE);
			bool InternalDrawShape(_In_ Algorithms::CRavidImage* pImage, _In_ CDC* pDC, _In_ ID2D1RenderTarget* pRender, _In_ CRavidThumbnailViewLayerElement* pRIVLE);

			bool InternalDrawPoint(_In_ Algorithms::CRavidImage* pImage, _In_ ID2D1RenderTarget* pRender, _In_ int &nGeometryIndex);
			bool InternalDrawLine(_In_ Algorithms::CRavidImage* pImage, _In_ ID2D1RenderTarget* pRender, _In_ int &nGeometryIndex);
			bool InternalDrawRectangle(_In_ Algorithms::CRavidImage* pImage, _In_ ID2D1RenderTarget* pRender, _In_ int &nGeometryIndex);
			bool InternalDrawQuadrangle(_In_ Algorithms::CRavidImage* pImage, _In_ ID2D1RenderTarget* pRender, _In_ int &nGeometryIndex);
			bool InternalDrawCircle(_In_ Algorithms::CRavidImage* pImage, _In_ ID2D1RenderTarget* pRender, _In_ int &nGeometryIndex);
			bool InternalDrawEllipse(_In_ Algorithms::CRavidImage* pImage, _In_ ID2D1RenderTarget* pRender, _In_ int &nGeometryIndex);
			bool InternalDrawPolygon(_In_ Algorithms::CRavidImage* pImage, _In_ ID2D1RenderTarget* pRender, _In_ int &nGeometryIndex);

			CRavidPoint<int> ImageCoordToScreenCoord(_In_ CRavidPoint<double> rpOffset, _In_ double dblScale, _In_ double fX, _In_ double fY);
			CRavidPoint<int> ImageCoordToScreenCoord(_In_ CRavidPoint<double> rpOffset, _In_ double dblScale, _In_ CRavidPoint<double>* pRpPoint);
			CRavidPoint<double> ScreenCoordToImageCoord(_In_ CRavidPoint<double> rpOffset, _In_ double dblScale, _In_ long nX, _In_ long nY);
			CRavidPoint<double> ScreenCoordToImageCoord(_In_ CRavidPoint<double> rpOffset, _In_ double dblScale, _In_ CRavidPoint<int>* pRpPoint);

			CRavidRect<int> ImageCoordToScreenCoord(_In_ CRavidPoint<double> rpOffset, _In_ double dblScale, _In_ double& fLeft, _In_ double& fTop, _In_ double& fRight, _In_ double& fBottom);
			CRavidRect<int> ImageCoordToScreenCoord(_In_ CRavidPoint<double> rpOffset, _In_ double dblScale, _In_ CRavidRect<double>* pRrRect);
			CRavidRect<double> ScreenCoordToImageCoord(_In_ CRavidPoint<double> rpOffset, _In_ double dblScale, _In_ long nLeft, _In_ long nTop, _In_ long nRight, _In_ long nBottom);
			CRavidRect<double> ScreenCoordToImageCoord(_In_ CRavidPoint<double> rpOffset, _In_ double dblScale, _In_ CRavidRect<int>* pRrRect);

			ID2D1Geometry* MakeD2DGeometry(_In_ Algorithms::CRavidImage* pImage, _In_ int &nGeometryIndex);
			ID2D1Geometry* MakeD2DGeometry(_In_ Algorithms::CRavidImage* pImage, _In_ CRavidGeometry* pRG);

			bool FigureD2DSink(_In_ Algorithms::CRavidImage* pImage, _In_ CRavidGeometry* pRG, _Inout_ ID2D1GeometrySink* pSink);
			bool FigureD2DSink(_In_ Algorithms::CRavidImage* pImage, _In_ int &nGeometryIndex, _Inout_ ID2D1GeometrySink* pSink);

		private:
			friend class CRavidThumbnailView;

		protected:
			std::vector<std::pair<Algorithms::CRavidImage*, SRavidDrawingStruct>> m_vctShapeItems;
			std::multimap<Algorithms::CRavidImage*, CRavidThumbnailViewLayerElement*> m_vctTextItems;

			CRavidObjectEx* m_pParentView = nullptr;

			bool m_bShow = true;
		};
	}
}