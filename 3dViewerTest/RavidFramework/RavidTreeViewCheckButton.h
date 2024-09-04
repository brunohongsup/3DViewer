
#ifndef RT_CHECKBUTTON
#define RT_CHECKBUTTON

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewCheckButton.h : header file
//

// Added Headers
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"

// Classes
class CRavidTreeView;

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewCheckButton window

class AFX_EXT_CLASS CRavidTreeViewCheckButton : public CWnd
{
// Construction
public:
	CRavidTreeViewCheckButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewCheckButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL GetOption(DWORD dwOption);
	void SetOption(DWORD dwOption, BOOL bSet);
	void SetCheckOptionsOwner(CRavidTreeView *otOption);
	BOOL GetCheck();
	void SetCheck(BOOL bCheck);
	CString GetCheckedText();
	CString GetUnCheckedText();
	void SetCheckText(CString strChecked, CString strUnChecked);
	virtual ~CRavidTreeViewCheckButton();

	// Generated message map functions
protected:
	CString m_strUnChecked;
	CString m_strChecked;
	BOOL m_bShowText;
	BOOL m_bShowCheck;
	BOOL m_bCheck;
	CRect m_rcCheck;
	CRavidTreeView *m_rtCheckOption;
	DWORD m_dwOptions;
	//{{AFX_MSG(CRavidTreeViewCheckButton)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_CHECKBUTTON
