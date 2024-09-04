
#ifndef RT_ITEMDATE
#define RT_ITEMDATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewItemDate.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"
#include "RavidTreeViewCheckButton.h"

// Classes
class CRavidTreeView;

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewItemDate window

class AFX_EXT_CLASS CRavidTreeViewItemDate : public CDateTimeCtrl, public CRavidTreeViewItem
{
// Construction
public:
	CRavidTreeViewItemDate();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewItemDate)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CreateDateItem(CString strFormat, DWORD dwDateStyle);
	virtual ~CRavidTreeViewItemDate();

	// Generated message map functions
protected:
	BOOL m_bFocus;
	//{{AFX_MSG(CRavidTreeViewItemDate)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_ITEMDATE
