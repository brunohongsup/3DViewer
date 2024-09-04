
#ifndef RT_ITEMHYPERLINK
#define RT_ITEMFONT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewItemFont.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"
#include "RavidTreeViewFontSel.h"

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewItemHyperLink window

class AFX_EXT_CLASS CRavidTreeViewItemHyperLink : public CWnd, public CRavidTreeViewItem
{
// Construction
public:
	CRavidTreeViewItemHyperLink();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);
	virtual void OnExpand(BOOL bExpand);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewItemHyperLink)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetVisited(BOOL bVisited);
	BOOL GetOption(DWORD dwOption);
	void SetOption(DWORD dwOption, BOOL bSet);
	void SetLink(CString strLink);
	CString GetLink();
	COLORREF GetVisitedColor();
	COLORREF GetHoverColor();
	COLORREF GetLinkColor();
	void SetVisitedColor(COLORREF crColor);
	void SetHoverColor(COLORREF crColor);
	void SetLinkColor(COLORREF crColor);
	BOOL GetVisited();
	HCURSOR GetLinkCursor();
	void SetLinkCursor(HCURSOR hCursor);
	BOOL CreateHyperlinkItem(DWORD dwOptions, CString strLink, COLORREF crLink, COLORREF crHover = NULL, COLORREF crVisited = NULL);
	virtual ~CRavidTreeViewItemHyperLink();

	// Generated message map functions
protected:
	BOOL m_bFocus;
	void SetDefaultCursor();
	COLORREF m_crLink;
	COLORREF m_crHover;
	COLORREF m_crVisited;
	BOOL m_bVisited;
	DWORD m_dwOptions;
	CString m_strLink;
	HCURSOR  m_hLinkCursor;
	CRect m_rcHover;
	BOOL m_bHover;
	CFont m_fUnderlineFont;
	//{{AFX_MSG(CRavidTreeViewItemHyperLink)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_ITEMHYPERLINK
