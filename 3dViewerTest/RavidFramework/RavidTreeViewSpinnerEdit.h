
#ifndef RT_SPINNEREDIT
#define RT_SPINNEREDIT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewSpinnerEdit.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"

// Classes
class CRavidTreeView;
class CRavidTreeViewSpinnerButton;

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewSpinnerEdit window

class AFX_EXT_CLASS CRavidTreeViewSpinnerEdit : public CEdit
{
// Construction
public:
	CRavidTreeViewSpinnerEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewSpinnerEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateMenu();
	void SetOwnerSpinner(CRavidTreeViewSpinnerButton *otSpinnerButton);
	virtual ~CRavidTreeViewSpinnerEdit();

	// Generated message map functions
protected:
	CRavidTreeViewSpinnerButton *m_rtSpinnerButton;
	//{{AFX_MSG(CRavidTreeViewSpinnerEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTextChange();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_SPINNEREDIT
