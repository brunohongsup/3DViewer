#pragma once

#include "RavidSheetCell.h"

namespace Ravid
{
	namespace Framework
	{
		typedef struct
		{
			LPCTSTR szURLPrefix;
			size_t  nLength;
		}
		URLStruct;

		class AFX_EXT_CLASS  CSheetCellCellURL : public CRavidSheetCell
		{
			DECLARE_DYNCREATE(CSheetCellCellURL)

		public:
			CSheetCellCellURL();
			virtual ~CSheetCellCellURL();

			virtual bool Draw(_In_ CDC* pDC, _In_ __int64 nRow, _In_ __int64 nCol, _In_ CRect rect, _In_opt_ bool bEraseBackground = true);
			virtual bool Edit(_In_ __int64 nRow, _In_ __int64 nCol, _In_ CRect rect, _In_ CPoint point, _In_ UINT nID, _In_ UINT nChar);
			virtual LPCTSTR  GetTipText() { return nullptr; }
			void SetAutoLaunchUrl(_In_opt_ bool bLaunch = true) { m_bLaunchUrl = bLaunch; }
			bool GetAutoLaunchUrl() { return m_bLaunchUrl; }

		protected:
			virtual bool OnSetCursor();
			virtual void OnClick(_In_ CPoint PointCellRelative);

			bool HasUrl(_In_ CString str);
			bool OverURL(_Out_ CPoint& pt, _Out_ CString& strURL);

		protected:
#ifndef _WIN32_WCE
			static HCURSOR g_hLinkCursor;
			HCURSOR GetHandCursor();
#endif
			static URLStruct g_szURIprefixes[];

		protected:
			COLORREF m_clrUrl;
			bool     m_bLaunchUrl;
			CRect    m_Rect;
		};
	}
}
