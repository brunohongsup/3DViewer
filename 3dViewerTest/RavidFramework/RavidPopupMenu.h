
// RavidPopupMenu.h : RavidPopupMenu 클래스의 인터페이스
//

#pragma once

namespace Ravid
{
	namespace Framework
	{
		class CRavidPopupMenuBar;

		class AFX_EXT_CLASS CRavidPopupMenu : public CMFCPopupMenu
		{
		public:
			CRavidPopupMenu();
			virtual ~CRavidPopupMenu();

			virtual void DoPaint(CDC* pPaintDC);

			virtual CMFCPopupMenuBar* GetMenuBar() { return (CMFCPopupMenuBar*)m_pWndNewMenuBar; }

			bool SetParentBtn(_In_ CMFCToolBarMenuButton* pParentBtn);

			bool SetDropDirection(_In_ DROP_DIRECTION dropDir);

			bool SetBackgroundColor(_In_ COLORREF clr);
			COLORREF GetBackgroundColor();

			bool SetButtonHoverBackgroundColor(_In_ COLORREF clr);
			COLORREF GetButtonHoverBackgroundColor();

			bool SetButtonClickBackgroundColor(_In_ COLORREF clr);
			COLORREF GetButtonClickBackgroundColor();

			bool SetButtonCheckedBackgroundColor(_In_ COLORREF clr);
			COLORREF GetButtonCheckedBackgroundColor();

		protected:
			CRavidPopupMenuBar* m_pWndNewMenuBar = nullptr;
		};
	}
}