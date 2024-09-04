
// RavidToolBarMenuButton.h : RavidToolBarMenuButton 클래스의 인터페이스
//

#pragma once

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidToolBarMenuButton : public CMFCToolBarMenuButton
		{
			DECLARE_SERIAL(CRavidToolBarMenuButton)

		public:
			CRavidToolBarMenuButton();
			CRavidToolBarMenuButton(UINT uiID, HMENU hMenu, int iImage, LPCTSTR lpszText = NULL, BOOL bUserButton = FALSE);
			virtual ~CRavidToolBarMenuButton();

			virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,
								BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE);

			virtual SIZE OnCalculateSize(CDC* pDC, const CSize& sizeDefault, BOOL bHorz);

		protected:
			void DrawMenuItem(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bCustomizeMode, BOOL bHighlight, BOOL bGrayDisabledButtons, BOOL bContentOnly = FALSE);
		};
	}
}