
#ifndef RT_LIST
#define RT_LIST

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewList window

// Added Headers
#include "RavidTreeViewDef.h"

// Classes
class CRavidTreeView;
class CRavidTreeViewItem;


class AFX_EXT_CLASS CRavidTreeViewList : public CWnd
{
// Construction
public:
	CRavidTreeViewList();
	BOOL Create(DWORD dwStyle, RECT rcRect, CWnd* pParentWnd, UINT nID);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewList)
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateResize();
	virtual ~CRavidTreeViewList();

protected:

	// Generated message map functions
protected:
	// CPropTree class that this class belongs
	CRavidTreeView *m_rtOption;
	long m_lPrevCol;
	BOOL m_bColDrag;
	HCURSOR m_hSplitter;
	HCURSOR m_hHand;
	long m_lColumn;
	//{{AFX_MSG(CRavidTreeViewList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void GetHandCursor();
	void SetOptionsOwner(CRavidTreeView *otOption);
	void CheckVisibleFocus();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	CRavidTreeViewItem* GetLastCtrlItem();

protected:
	CRavidTreeViewItem* m_pLastClickOTI = nullptr;

	bool m_bIsMultiKeyDown = false;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_LIST
