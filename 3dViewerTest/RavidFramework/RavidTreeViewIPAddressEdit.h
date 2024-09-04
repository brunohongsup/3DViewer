
#ifndef RT_ITEMIPADDRESSEDIT
#define RT_ITEMIPADDRESSEDIT


// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"

// Classes
class CRavidTreeView;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewIPAddressEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewIPAddressEdit window

class AFX_EXT_CLASS CRavidTreeViewIPAddressEdit : public CEdit
{
// Construction
public:
	CRavidTreeViewIPAddressEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewIPAddressEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetNotifyWnd(HWND hWnd);
	virtual ~CRavidTreeViewIPAddressEdit();

	// Generated message map functions
protected:
	HWND m_wndNotify;
	//{{AFX_MSG(CRavidTreeViewIPAddressEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMaxText();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_ITEMIPADDRESSEDIT
