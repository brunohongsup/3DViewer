
// RavidPopupMenuBar.h : RavidPopupMenuBar 클래스의 인터페이스
//

#pragma once

#include <memory>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidPopupMenuBar : public CMFCPopupMenuBar
		{
		public:
			CRavidPopupMenuBar();
			virtual ~CRavidPopupMenuBar();

			virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

			void UpdateButton(int nIndex);

			virtual BOOL PreTranslateMessage(MSG* pMsg);

			bool SetBackgroundColor(_In_ COLORREF clr);
			COLORREF GetBackgroundColor();

			bool SetButtonHoverBackgroundColor(_In_ COLORREF clr);
			COLORREF GetButtonHoverBackgroundColor();

			bool SetButtonClickBackgroundColor(_In_ COLORREF clr);
			COLORREF GetButtonClickBackgroundColor();

			bool SetButtonCheckedBackgroundColor(_In_ COLORREF clr);
			COLORREF GetButtonCheckedBackgroundColor();

		protected:
			virtual CSize CalcSize(BOOL bVertDock);

			virtual void DoPaint(CDC* pDC);

			virtual void OnFillBackground(CDC* pDC);

			virtual BOOL DrawButton(CDC* pDC, CMFCToolBarButton* pButton, CMFCToolBarImages* pImages, BOOL bHighlighted, BOOL bDrawDisabledImages);
			
		public:
			DECLARE_MESSAGE_MAP()
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

		protected:
			COLORREF m_clrBackground = 0;
			COLORREF m_clrButtonHoverBackground = 0;
			COLORREF m_clrButtonClickBackground = 0;
			COLORREF m_clrButtonCheckedBackground = 0;
		};
	}
}
