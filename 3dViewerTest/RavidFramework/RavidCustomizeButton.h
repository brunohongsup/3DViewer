
// RavidCustomizeButton.h : RavidCustomizeButton 클래스의 인터페이스
//

#pragma once

#include "afxcustomizebutton.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidCustomizeButton : public CMFCCustomizeButton
		{
			RavidPreventCopySelf(CRavidCustomizeButton);

		public:
			CRavidCustomizeButton();
			virtual ~CRavidCustomizeButton();

			virtual CMFCPopupMenu* CreatePopupMenu();
			virtual BOOL OpenPopupMenu(CWnd* pWnd = NULL);

			bool SetParentToolbar(_In_ CMFCToolBar* pParent);

			bool SetInvisibleButtons(_In_ const CObList& obList);

			bool SetShowAtRightSide(_In_ bool bRight);

			bool SetParentBtn(_In_ CMFCToolBarMenuButton* pParentBtn);

			bool SetParentWnd(_In_ CWnd* pWnd);

			bool SetMenuBackgroundColor(_In_ COLORREF clr);
			COLORREF GetMenuBackgroundColor();

			bool SetMenuButtonHoverBackgroundColor(_In_ COLORREF clr);
			COLORREF GetMenuButtonHoverBackgroundColor();

			bool SetMenuButtonClickBackgroundColor(_In_ COLORREF clr);
			COLORREF GetMenuButtonClickBackgroundColor();

			bool SetMenuButtonCheckedBackgroundColor(_In_ COLORREF clr);
			COLORREF GetMenuButtonCheckedBackgroundColor();

		protected:
			CMFCToolBarMenuButton* m_pParentBtn = nullptr;

			COLORREF m_clrMenuBackground = 0;
			COLORREF m_clrMenuButtonHoverBackground = 0;
			COLORREF m_clrMenuButtonClickBackground = 0;
			COLORREF m_clrMenuButtonCheckedBackground = 0;
		};
	}
}