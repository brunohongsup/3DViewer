
#ifndef RT_ITEMSPINNER
#define RT_ITEMSPINNER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewItemSpinner.h : header file
//


// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"
#include "RavidTreeViewSpinnerButton.h"

// Classes
class CRavidTreeView;


/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewItemSpinner window

class AFX_EXT_CLASS CRavidTreeViewItemSpinner : public CRavidTreeViewSpinnerButton, public CRavidTreeViewItem
{
// Construction
public:
	CRavidTreeViewItemSpinner();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);
	virtual void ActivateCurrent();

	virtual void ChangeValue();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewItemSpinner)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CreateSpinnerItem(DWORD dwOptions, double dValue, double dRangeBottom, double dRangeTop);
	virtual ~CRavidTreeViewItemSpinner();

protected:
	LRESULT WM_CommitChanges(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
protected:
	BOOL m_bFocus;

	//{{AFX_MSG(CRavidTreeViewItemSpinner)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_ITEMSPINNER
