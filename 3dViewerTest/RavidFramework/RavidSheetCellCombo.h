#pragma once

#include "RavidSheetCell.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidSheetCellCombo : public CRavidSheetCell
		{
			DECLARE_DYNCREATE(CRavidSheetCellCombo)

		public:
			CRavidSheetCellCombo();

		public:
			virtual bool  Edit(_In_ __int64 nRow, _In_ __int64 nCol, _In_ CRect rect, _In_ CPoint point, _In_ UINT nID, _In_ UINT nChar);
			virtual CWnd* GetCtrlWnd();
			virtual void  EndEdit();

		public:
			virtual CSize GetCellExtent(_In_ CDC* pDC);

		public:
			void  SetOptions(_In_ CStringArray& ar);
			void  SetStyle(_In_ DWORD dwStyle) { m_dwStyle = dwStyle; }
			DWORD GetStyle() { return m_dwStyle; }

			bool SetCurSel(_In_ int nIdx);
			int GetCurSel();

		protected:
			virtual bool Draw(_In_ CDC* pDC, _In_ __int64 nRow, _In_ __int64 nCol, _In_ CRect rect, _In_opt_ bool bEraseBackground = true);

			virtual void OnClick(_In_ CPoint PointCellRelative);

			CStringArray m_Strings;
			DWORD        m_dwStyle;
		};


#define IDC_COMBOEDIT 1001

		class CComboEdit : public CEdit
		{
		public:
			CComboEdit();

		public:

		public:

			virtual BOOL PreTranslateMessage(MSG* pMsg);

		public:
			virtual ~CComboEdit();

		protected:
			afx_msg void OnKillFocus(CWnd* pNewWnd);
			afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

			DECLARE_MESSAGE_MAP()
		};


		class CInPlaceList : public CComboBox
		{
			friend class CComboEdit;

		public:
			CInPlaceList(CWnd* pParent,
						 CRect& rect,
						 DWORD dwStyle,
						 UINT nID,
						 int nRow, int nColumn,
						 COLORREF crFore, COLORREF crBack,
						 CStringArray& Items,
						 CString sInitText,
						 UINT nFirstChar);

		public:
			CComboEdit m_edit;

		public:

		protected:
			virtual void PostNcDestroy();

		public:
			virtual ~CInPlaceList();
			void EndEdit();

		protected:
			int GetCorrectDropWidth();

		protected:
			afx_msg void OnKillFocus(CWnd* pNewWnd);
			afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnDropdown();
			afx_msg UINT OnGetDlgCode();
			afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

			DECLARE_MESSAGE_MAP()

		private:
			int		 m_nNumLines;
			CString  m_sInitText;
			int		 m_nRow;
			int		 m_nCol;
			UINT     m_nLastChar;
			BOOL	 m_bExitOnArrows;
			COLORREF m_crForeClr, m_crBackClr;

		public:
			afx_msg void OnCbnSelchange();
			afx_msg void OnCbnSelendcancel();
			afx_msg void OnPaint();
		};
	}
}
