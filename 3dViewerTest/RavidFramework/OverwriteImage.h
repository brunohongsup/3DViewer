#pragma once

#include "RavidObjectEx.h"

#include "../RavidCore/RavidGeometry.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Framework
	{
		class CRavidImageViewLayer;

		class AFX_EXT_CLASS COverwriteImage : public CRavidObjectEx
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(COverwriteImage);

		public:
			COverwriteImage();
			virtual ~COverwriteImage();

			void Clear();

			bool AddShape(_In_ CRavidGeometry& rgObject, _In_opt_ COLORREF crLineColor = 0, _In_opt_ long nLineWidth = 1, _In_opt_ COLORREF crFillColor = ERavidImageViewLayerTransparencyColor, _In_opt_ ERavidImageViewPenStyle ePenStyle = ERavidImageViewPenStyle_SOLID, _In_opt_ float fLineColorAlphaRatio = 1.f, _In_opt_ float fFillColorAlphaRatio = 1.f);
			bool AddShape(_In_ CRavidGeometry* pRgObject, _In_opt_ COLORREF crLineColor = 0, _In_opt_ long nLineWidth = 1, _In_opt_ COLORREF crFillColor = ERavidImageViewLayerTransparencyColor, _In_opt_ ERavidImageViewPenStyle ePenStyle = ERavidImageViewPenStyle_SOLID, _In_opt_ float fLineColorAlphaRatio = 1.f, _In_opt_ float fFillColorAlphaRatio = 1.f);
			bool AddShape(_In_ SRavidDrawingStruct* pShapeElement);

			bool AddText(_In_ CRavidGeometry& rgObject, _In_ CString strText, _In_opt_ COLORREF crTextColor = 0, _In_opt_ COLORREF crFillColor = ERavidImageViewLayerTransparencyColor, _In_opt_ long nFontSize = 12, _In_opt_ double dblAngle = 0., _In_opt_ DWORD dwAlign = TA_LEFT | TA_TOP, _In_opt_ CString* pFontName = nullptr, _In_opt_ float fTextColorAlphaRatio = 1.f, _In_opt_ float fFillColorAlphaRatio = 1.f);
			bool AddText(_In_ CRavidGeometry* pRgObject, _In_ CString strText, _In_opt_ COLORREF crTextColor = 0, _In_opt_ COLORREF crFillColor = ERavidImageViewLayerTransparencyColor, _In_opt_ long nFontSize = 12, _In_opt_ double dblAngle = 0., _In_opt_ DWORD dwAlign = TA_LEFT | TA_TOP, _In_opt_ CString* pFontName = nullptr, _In_opt_ float fTextColorAlphaRatio = 1.f, _In_opt_ float fFillColorAlphaRatio = 1.f);

			bool Execute(_In_ Ravid::Algorithms::CRavidImage* pSrcImage, _Out_ Ravid::Algorithms::CRavidImage* pDstImage);

		protected:
			CRavidImageViewLayer* m_pLayer;
		};
	}
}

