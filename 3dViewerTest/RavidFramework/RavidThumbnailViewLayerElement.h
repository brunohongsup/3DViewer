#pragma once

#include "RavidObjectEx.h"
#include "../RavidCore/RavidRect.h"

class CRavidGeometry;

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidThumbnailViewLayerElement : public CRavidObjectEx
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CRavidThumbnailViewLayerElement);

		public:

			CRavidThumbnailViewLayerElement();
			virtual ~CRavidThumbnailViewLayerElement();

			void Clear();
			void ClearShape();
			void ClearText();
			void ClearFontName();
			void ClearFont();

			bool SetShape(_In_ CRavidGeometry& rgShape);
			bool SetShape(_In_ CRavidGeometry* pRgShape);
			CRavidGeometry* GetShape();

			void SetText(_In_ CString strText);
			CString* GetText();

			void SetLineWidth(_In_ long nLineWidth);
			long GetLineWidth();

			void SetLineLength(_In_ long nLineLength);
			long GetLineLength();

			void SetLineColor(_In_ COLORREF crLineColor);
			COLORREF GetLineColor();

			void SetTextColor(_In_ COLORREF crTextColor);
			COLORREF GetTextColor();

			void SetDiagonal(bool bDiagonal);
			bool IsDiagonal();

			void SetFillColor(_In_ COLORREF crFillColor);
			void ClearFillColor();
			COLORREF GetFillColor();

			void SetFontSize(_In_ long nSize);
			long GetFontSize();

			void SetFontName(_In_ CString strFontName);
			CString* GetFontName();

			bool CreateFont(_In_ Algorithms::CRavidImage* pImage, _In_ CRavidObjectEx* pView, _In_opt_ CDC* pDC = nullptr);
			CFont* GetFont();

			bool ModifyFont(_In_ Algorithms::CRavidImage* pImage, _In_ CRavidObjectEx* pView, _In_opt_ CDC* pDC = nullptr);

			IDWriteTextFormat* GetDWriteTextFormat();

			IDWriteTextLayout* GetDWriteTextLayout();

			CSize GetTextExtent();

			void SetRealSizeFont(_In_ bool bSet);
			bool IsRealSizeFont();

			void SetFontAngle(_In_ double dblAngle);
			double GetFontAngle();

			void SetTextAlign(_In_ DWORD dwAlign);
			DWORD GetTextAlign();

			void SetPenStyle(_In_ ERavidImageViewPenStyle ePenStyle);
			ERavidImageViewPenStyle GetPenStyle();

			void SetLineColorAlphaBlendingRatio(_In_ float fAlphaBlendingRatio);
			float GetLineColorAlphaBlendingRatio();

			void SetFillColorAlphaBlendingRatio(_In_ float fAlphaBlendingRatio);
			float GetFillColorAlphaBlendingRatio();

			void SetTextColorAlphaBlendingRatio(_In_ float fAlphaBlendingRatio);
			float GetTextColorAlphaBlendingRatio();

		protected:
			friend class CRavidImageViewLayer;
			friend class CRavidThumbnailViewLayer;

		protected:
			CRavidGeometry* m_pShape = nullptr;
			CRavidRect<double> m_rect;
			CString* m_pStrText = nullptr;
			CString* m_pStrFontName = nullptr;
			COLORREF m_crLineColor = 0;
			COLORREF m_crFillColor = ERavidImageViewLayerTransparencyColor;
			long m_nFontSize = 12;
			bool m_bRealSizeFont = false;
			double m_dblFontAngle = 0.;
			DWORD m_dwTextAlign = TA_LEFT | TA_TOP;
			long m_nLineWidth = 1;
			long m_nLineLength = 1;
			bool m_bDiagonal = false;
			ERavidImageViewPenStyle m_ePenStyle = ERavidImageViewPenStyle_SOLID;
			float m_fLineColorAlphaBlendingRatio = 1.f;
			float m_fFillColorAlphaBlendingRatio = 1.f;

			CFont* m_pFont = nullptr;
			CSize m_textExtent;

			IDWriteTextFormat* m_pDWriteTextFormat = nullptr;
			IDWriteTextLayout* m_pDWriteTextLayout = nullptr;

			double m_dblPrevScale = 0.;
		};
	}
}

