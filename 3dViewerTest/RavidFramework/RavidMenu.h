
// RavidMenu.h : CRavidMenu 클래스의 인터페이스
//
#pragma once

#include <vector>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidMenu : public CMenu
		{
			
			RavidPreventCopySelf(CRavidMenu);
		public:
			CRavidMenu();
			virtual ~CRavidMenu();

			bool Initialize(_In_ CWnd* pParent, _In_ COLORREF clrBackground, _In_ COLORREF clrItemNomal, _In_ COLORREF clrItemHotlight, _In_ COLORREF clrItemSelected, _In_ COLORREF clrItemNomalText, _In_ COLORREF clrItemDisabledText);
			bool IsInitialize();

			bool SetBackgroundColor(_In_ COLORREF clr);
			COLORREF GetBackgroundColor();

			bool SetItemNomalColor(_In_ COLORREF clr);
			COLORREF GetItemNomalColor();

			bool SetItemHotlightColor(_In_ COLORREF clr);
			COLORREF GetItemHotlightColor();

			bool SetItemSelectedColor(_In_ COLORREF clr);
			COLORREF GetItemSelectedColor();

			bool SetItemNomalTextColor(_In_ COLORREF clr);
			COLORREF GetItemNomalTextColor();

			bool SetItemDisabledTextColor(_In_ COLORREF clr);
			COLORREF GetItemDisabledTextColor();

			bool SetDrawTextFormat(_In_ DWORD dwFormat);
			DWORD GetDrawTextFormat();

			bool UseIconColorChange(_In_ bool bUse);
			bool IsUsedIconColorChange();

			bool SetChangeIconTargetColor(_In_ COLORREF clr);
			COLORREF GetChangeIconTargetColor();

			bool SetIconTransparentColor(_In_ COLORREF clr);
			COLORREF GetIconTransparentColor();



			virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
			virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

			BOOL TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect = 0);

		private:
			HBITMAP ConversionIcon(_In_ HBITMAP hBmp, _In_ long nHeight, _In_ COLORREF clrTransparent, _In_ COLORREF clrBackground, _In_opt_ bool bUseObjectColorChange = false, _In_opt_ COLORREF clrTarget = 0, _In_opt_ COLORREF clrChange = 0);
			HBITMAP CreateBitmap32(_In_ HBITMAP hBmp, _In_opt_ COLORREF clrTransparent = -1);
			HBITMAP CreateBitmap32(_In_ CSize& size, _Out_ void** pBits);

			
		protected:
			bool m_bIsInitialize = false;

			std::vector<SRavidMenuItemInfo> m_vctMenuItems;

			std::vector<CRavidMenu*> m_vctSubMenu;

			CBrush* m_pBrushBackground = nullptr;

			CWnd* m_pParent = nullptr;

			COLORREF m_clrBackground = RGB(0, 0, 0);

			COLORREF m_clrItemNomal = RGB(0, 0, 0);
			COLORREF m_clrItemHotlight = RGB(0, 0, 0);
			COLORREF m_clrItemSelected = RGB(0, 0, 0);

			COLORREF m_clrItemNomalText = RGB(0, 0, 0);
			COLORREF m_clrItemDisabledText = RGB(0, 0, 0);

			DWORD m_dwDrawTextFormat = DT_SINGLELINE | DT_VCENTER | DT_CENTER;

			CPoint m_ptOffset = CPoint(0, 0);
			UINT m_nFlag = 0;

			HBITMAP m_hCheckBmp = nullptr;

			bool m_bUseMenuBitmap = false;
			bool m_bUseCheck = false;

			float m_fBitmapOffsetCoef = 1.2f;
			long m_nDrawTextStartOffset = 5;

			bool m_bUseIconColorChange = false;

			COLORREF m_clrChangeIconTarget = RGB(0, 0, 0);
			COLORREF m_clrIconTransparent = RGB(255, 255, 255);
		};
	}
}
