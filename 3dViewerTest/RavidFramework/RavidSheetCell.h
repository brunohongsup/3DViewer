#pragma once

#include "RavidSheetCellBase.h"

namespace Ravid
{
	namespace Framework
	{
		class CRavidSheet;

		class AFX_EXT_CLASS CRavidSheetCell : public CRavidSheetCellBase
		{
			friend class CRavidSheet;
			DECLARE_DYNCREATE(CRavidSheetCell)

		public:
			CRavidSheetCell();
			virtual ~CRavidSheetCell();

		public:
			void operator=(_In_ CRavidSheetCell& cell);

			virtual void  SetText(_In_ LPCTSTR lpszText) { m_strText = lpszText; }
			virtual void  SetImage(_In_ int nImage) { m_nImage = nImage; }
			virtual void  SetData(_In_ LPARAM lParam) { m_lParam = lParam; }
			virtual void  SetGrid(_In_ CRavidSheet* pSheet) { m_pGrid = pSheet; }
			virtual void  SetFormat(_In_ DWORD dwFormat) { m_nFormat = dwFormat; }
			virtual void  SetTextClr(_In_ COLORREF clr) { m_crFgClr = clr; }
			virtual void  SetBackClr(_In_ COLORREF clr) { m_crBkClr = clr; }
			virtual void  SetFont(_In_ LOGFONT* plf);
			virtual void  SetMargin(_In_ UINT uiMargin) { m_nMargin = uiMargin; }
			virtual CWnd* GetCtrlWnd() { return m_pCtrlWnd; }
			virtual void  SetCoords(_In_ __int64 nRow, _In_ __int64 nCol) { }

			virtual LPCTSTR     GetText() { return (m_strText.IsEmpty()) ? _T("") : LPCTSTR(m_strText); }
			virtual int         GetImage() { return m_nImage; }
			virtual LPARAM      GetData() { return m_lParam; }
			virtual CRavidSheet*  GetGrid() { return m_pGrid; }
			virtual DWORD       GetFormat();
			virtual COLORREF    GetTextClr() { return m_crFgClr; }
			virtual COLORREF    GetBackClr() { return m_crBkClr; }
			virtual LOGFONT*    GetFont();
			virtual CFont*      GetFontObject();
			virtual UINT        GetMargin();

			virtual bool        IsEditing() { return m_bEditing; }
			virtual bool        IsDefaultFont() { return (m_plfFont == NULL); }
			virtual void        Reset();

		public:
			virtual bool Edit(_In_ __int64 nRow, _In_ __int64 nCol, _In_ CRect rect, _In_ CPoint point, _In_ UINT nID, _In_ UINT nChar);
			virtual void EndEdit();
		protected:
			virtual void OnEndEdit();

		protected:
			CString    m_strText;
			LPARAM     m_lParam;
			int        m_nImage;
			DWORD      m_nFormat;
			COLORREF   m_crFgClr;
			COLORREF   m_crBkClr;
			LOGFONT*   m_plfFont;
			UINT       m_nMargin;

			bool       m_bEditing;

			CRavidSheet* m_pGrid; 
			CWnd*      m_pCtrlWnd;
		};

		class CRavidSheetDefaultCell : public CRavidSheetCell
		{
			DECLARE_DYNCREATE(CRavidSheetDefaultCell)

		public:
			CRavidSheetDefaultCell();
			virtual ~CRavidSheetDefaultCell();

		public:
			virtual DWORD GetStyle() { return m_dwStyle; }
			virtual void  SetStyle(DWORD dwStyle) { m_dwStyle = dwStyle; }
			virtual int   GetWidth() { return m_Size.cx; }
			virtual int   GetHeight() { return m_Size.cy; }
			virtual void  SetWidth(int nWidth) { m_Size.cx = nWidth; }
			virtual void  SetHeight(int nHeight) { m_Size.cy = nHeight; }

			virtual void     SetData(LPARAM /*lParam*/) { ASSERT(FALSE); }
			virtual void     SetState(DWORD /*nState*/) { ASSERT(FALSE); }
			virtual DWORD    GetState() { return CRavidSheetCell::GetState() | ERavidSheetCellStatus_ReadOnly; }
			virtual void     SetCoords(int /*row*/, int /*col*/) { ASSERT(FALSE); }
			virtual void     SetFont(LOGFONT* /*plf*/);
			virtual LOGFONT* GetFont();
			virtual CFont*   GetFontObject();

		protected:
			CSize m_Size;
			CFont m_Font;
			DWORD m_dwStyle;
		};
	}
}