#pragma once

#include "RavidSheetCell.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidSheetCellCheck : public CRavidSheetCell
		{
			friend class CGridCtrl;
			DECLARE_DYNCREATE(CRavidSheetCellCheck)

		public:
			CRavidSheetCellCheck();

		public:
			virtual void SetText(_In_ LPCTSTR lpszText);
			virtual bool Edit(_In_ __int64 nRow, _In_ __int64 nCol, _In_ CRect rect, _In_ CPoint point, _In_ UINT nID, _In_ UINT nChar);


			bool SetCheck(_In_opt_ bool bChecked = true);
			bool GetCheck();

			virtual CSize GetCellExtent(_In_ CDC* pDC);
			virtual void OnClick(_In_ CPoint PointCellRelative);
			virtual bool GetTextRect(_Out_ LPRECT pRect);

		protected:
			CRect GetCheckPlacement();

			virtual bool Draw(_In_ CDC* pDC, _In_ __int64 nRow, _In_ __int64 nCol, _In_ CRect rect, _In_opt_ bool bEraseBackground = true);

		protected:
			DWORD m_dwStyle = 0x0000;

			bool  m_bChecked;
			CRect m_Rect;
		};
	}
}
