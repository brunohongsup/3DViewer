#pragma once

#include "RavidSheetCell.h"

#include "afxdtctl.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidSheetCellDateTime : public CRavidSheetCell
		{
			friend class CGridCtrl;
			DECLARE_DYNCREATE(CRavidSheetCellDateTime)

			CTime m_cTime;
			DWORD m_dwStyle;

		public:
			CRavidSheetCellDateTime();
			CRavidSheetCellDateTime(DWORD dwStyle);
			virtual ~CRavidSheetCellDateTime();
			virtual CSize GetCellExtent(_In_ CDC* pDC);

		public:
			void Init(_In_ DWORD dwStyle);
			virtual bool Edit(_In_ __int64 nRow, _In_ __int64 nCol, _In_ CRect rect, _In_ CPoint point, _In_ UINT nID, _In_ UINT nChar);
			virtual CWnd* GetEditWnd() const;
			virtual void  EndEdit();


			CTime* GetTime() { return &m_cTime; };
			void   SetTime(_In_ CTime time);
		};

		class AFX_EXT_CLASS CRavidSheetInPlaceDateTime : public CDateTimeCtrl
		{
		public:
			CRavidSheetInPlaceDateTime(CWnd* pParent,
									   CRect& rect,
									   DWORD dwStyle,
									   UINT nID,
									   int nRow, int nColumn,
									   COLORREF crFore, COLORREF crBack,
									   CTime* pcTime,
									   UINT nFirstChar);

		protected:
			virtual void PostNcDestroy();

		public:
			virtual ~CRavidSheetInPlaceDateTime();
			void EndEdit();

		protected:
			afx_msg void OnKillFocus(CWnd* pNewWnd);
			afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg UINT OnGetDlgCode();

			DECLARE_MESSAGE_MAP()

		private:
			CTime*   m_pcTime;
			int		 m_nRow;
			int		 m_nCol;
			UINT     m_nLastChar;
			BOOL	 m_bExitOnArrows;
			COLORREF m_crForeClr, m_crBackClr;
		};

	}
}
