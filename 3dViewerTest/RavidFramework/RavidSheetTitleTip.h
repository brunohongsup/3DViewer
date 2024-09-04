#pragma once

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidSheetTitleTip : public CWnd
		{
		public:
			CRavidSheetTitleTip();
			virtual ~CRavidSheetTitleTip();

			virtual BOOL Create(CWnd* pParentWnd);
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual BOOL DestroyWindow();

		public:
			void SetParentWnd(_In_ CWnd* pParentWnd);
			CWnd* GetParentWnd();

			void Show(_In_ CRect crTitle, _In_ LPCTSTR lpszTitleText, _In_opt_ int nXOffset = 0, _In_opt_ LPRECT lpHoverRect = NULL, _In_opt_ LOGFONT* lpLogFont = NULL, _In_opt_ COLORREF clrText = CLR_DEFAULT, _In_opt_ COLORREF clrBack = CLR_DEFAULT);
			void Hide();

		protected:
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			DECLARE_MESSAGE_MAP()

		protected:
			CWnd*  m_pParentWnd = nullptr;
			CRect  m_crTitle = CRect(0, 0, 0, 0);
			CRect  m_crHover = CRect(0, 0, 0, 0);
			DWORD  m_dwLastLButtonDown = ULONG_MAX;
			DWORD  m_dwDblClickMsecs = GetDoubleClickTime();
			bool   m_bIsCreated = false;
		};
	}
}
