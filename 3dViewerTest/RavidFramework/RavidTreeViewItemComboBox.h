
#ifndef RT_ITEMCOMBOBOX
#define RT_ITEMCOMBOBOX

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewItemComboBox.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"

// Classes
class CRavidTreeView;

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewItemComboBox window

class AFX_EXT_CLASS CRavidTreeViewItemComboBox : public CComboBox, public CRavidTreeViewItem
{
// Construction
public:
	CRavidTreeViewItemComboBox();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);

	virtual void ActivateCurrent();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewItemComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	long GetDropDownHeight();
	void SetDropDownHeight(long lHeight);
	BOOL CreateComboItem(DWORD dwAddStyle = 0);
	virtual ~CRavidTreeViewItemComboBox();

	// Generated message map functions
protected:
	BOOL m_bFocus;
	long m_lDropDownHeight;
	int m_preSelect;
	//{{AFX_MSG(CRavidTreeViewItemComboBox)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelChange();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_ITEMCOMBOBOX
