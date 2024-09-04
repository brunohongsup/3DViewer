#pragma once

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidSheetInPlaceEdit : public CEdit
		{
		public:
			CRavidSheetInPlaceEdit(_In_ CWnd* pParent, _In_ CRect& rect, _In_ DWORD dwStyle, _In_ UINT uiID, _In_ __int64 nRow, _In_ __int64 nColumn, _In_ CString strInitText, _In_ UINT uiFirstChar);
			virtual ~CRavidSheetInPlaceEdit();

			virtual BOOL PreTranslateMessage(MSG* pMsg);

		public:
			void EndEdit();

			void SetNumeric(_In_ bool bSet);
			bool IsNumeric();

		protected:
			virtual void PostNcDestroy();
			afx_msg void OnKillFocus(CWnd* pNewWnd);
			afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg UINT OnGetDlgCode();
			DECLARE_MESSAGE_MAP()

		private:
			__int64 m_nRow = 0;
			__int64 m_nColumn = 0;
			CString m_strInitText = _T("");
			UINT    m_uiLastChar = 0;
			bool    m_bIsExitOnArrows = false;
			CRect   m_cr = CRect(0, 0, 0, 0);

			bool m_bIsNumeric = false;
		};
	}
}

