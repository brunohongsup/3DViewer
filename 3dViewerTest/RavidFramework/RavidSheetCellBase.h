#pragma once

namespace Ravid
{
	namespace Framework
	{
		class CRavidSheet;

		class AFX_EXT_CLASS CRavidSheetCellBase : public CObject
		{
			friend class CRavidSheet;
			DECLARE_DYNAMIC(CRavidSheetCellBase)

		public:
			CRavidSheetCellBase();
			virtual ~CRavidSheetCellBase();

		public:
			virtual void SetText(_In_ LPCTSTR lpszText) = 0;
			virtual void SetImage(_In_ int nImage) = 0;
			virtual void SetData(_In_ LPARAM lParam) = 0;
			virtual void SetState(_In_ DWORD dwState);
			virtual void SetFormat(_In_ DWORD nFormat) = 0;
			virtual void SetTextClr(_In_ COLORREF clr) = 0;
			virtual void SetBackClr(_In_ COLORREF clr) = 0;
			virtual void SetFont(_In_ LOGFONT* pLoFont) = 0;
			virtual void SetMargin(_In_ UINT uiMargin) = 0;
			virtual void SetGrid(_In_ CRavidSheet* pSheet) = 0;
			virtual void SetCoords(_In_ __int64 nRow, _In_ __int64 nCol) = 0;

			virtual LPCTSTR			GetText() = 0;
			virtual LPCTSTR			GetTipText() { return GetText(); }
			virtual int				GetImage() = 0;
			virtual LPARAM			GetData() = 0;
			virtual DWORD			GetState() { return m_nState; }
			virtual DWORD			GetFormat() = 0;
			virtual COLORREF		GetTextClr() = 0;
			virtual COLORREF		GetBackClr() = 0;
			virtual LOGFONT*		GetFont() = 0;
			virtual CFont*			GetFontObject() = 0;
			virtual CRavidSheet*	GetGrid() = 0;
			virtual CWnd*			GetCtrlWnd() = 0;
			virtual UINT			GetMargin() = 0;

			virtual CRavidSheetCellBase* GetDefaultCell();

			virtual bool IsDefaultFont() = 0;
			virtual bool IsEditing() = 0;
			virtual bool IsFocused();
			virtual bool IsFixed();
			virtual bool IsFixedCol();
			virtual bool IsFixedRow();
			virtual bool IsSelected();
			virtual bool IsReadOnly();
			virtual bool IsModified();
			virtual bool IsDropHighlighted();

		public:
			virtual void operator=(_In_ CRavidSheetCellBase& cell);

		public:
			virtual void Reset();

			virtual bool Draw(_In_ CDC* pDC, _In_ __int64 nRow, _In_ __int64 nCol, _In_ CRect rect, _In_opt_ bool bEraseBackground = true);
			virtual bool GetTextRect(_Out_ LPRECT pRect);
			virtual bool GetTipTextRect(_Out_ LPRECT pRect);
			virtual CSize GetTextExtent(_In_ LPCTSTR str, _In_opt_ CDC* pDC = nullptr);
			virtual CSize GetCellExtent(_In_ CDC* pDC);

			virtual bool Edit(_In_ __int64 nRow, _In_ __int64 nCol, _In_ CRect rect, _In_ CPoint point, _In_ UINT nID, _In_ UINT nChar);
			virtual bool ValidateEdit(_In_ LPCTSTR str);
			virtual void EndEdit();

			virtual bool PrintCell(_In_ CDC* pDC, _In_ __int64 nRow, _In_ __int64 nCol, _In_ CRect rect);

			LRESULT SendMessageToParent(_In_ __int64 nRow, _In_ __int64 nCol, UINT uiMessage);

		protected:
			virtual void OnEndEdit();
			virtual void OnMouseEnter();
			virtual void OnMouseOver();
			virtual void OnMouseLeave();
			virtual void OnClick(_In_ CPoint PointCellRelative);
			virtual void OnClickDown(_In_ CPoint PointCellRelative);
			virtual void OnRClick(_In_ CPoint PointCellRelative);
			virtual void OnDblClick(_In_ CPoint PointCellRelative);
			virtual bool OnSetCursor();

		protected:
			DWORD    m_nState;
		};
	}
}
