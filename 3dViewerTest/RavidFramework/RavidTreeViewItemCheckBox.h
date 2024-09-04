
#ifndef RT_ITEMCHECKBOX
#define RT_ITEMCHECKBOX

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewItemCheckBox.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"
#include "RavidTreeViewCheckButton.h"

// Classes
class CRavidTreeView;

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewItemCheckBox window

class AFX_EXT_CLASS CRavidTreeViewItemCheckBox : public CRavidTreeViewCheckButton, public CRavidTreeViewItem
{
// Construction
public:
	CRavidTreeViewItemCheckBox();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);
	virtual void OnDeSelect();
	virtual void OnSelect();

	BOOL CreateCheckBoxItem(BOOL bChecked, DWORD dwOptions);

	virtual void ActivateCurrent();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewItemCheckBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRavidTreeViewItemCheckBox();

	// Generated message map functions
protected:
	BOOL m_bFocus;
	CRect m_rcCheck;
	//{{AFX_MSG(CRavidTreeViewItemCheckBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_ITEMCHECKBOX
