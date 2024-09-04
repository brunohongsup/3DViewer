
#ifndef RT_ITEMIPADDRESS
#define RT_ITEMIPADDRESS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewItemFont.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"
#include "RavidTreeViewIPAddressEdit.h"

// Classes
class CRavidTreeView;


/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewItemIPAddress window

class AFX_EXT_CLASS CRavidTreeViewItemIPAddress : public CIPAddressCtrl, public CRavidTreeViewItem
{
// Construction
public:
	CRavidTreeViewItemIPAddress();
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
	//{{AFX_VIRTUAL(CRavidTreeViewItemIPAddress)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CreateIPAddressItem(DWORD dwAddStyle);
	virtual ~CRavidTreeViewItemIPAddress();

	// Generated message map functions
protected:
	afx_msg LRESULT OnEditLooseFocus(WPARAM lParam, LPARAM wParam);
	afx_msg LRESULT OnEditNextEdit(WPARAM lParam, LPARAM wParam);	
	afx_msg LRESULT OnSelectNext(WPARAM lParam, LPARAM wParam);
	afx_msg LRESULT OnSelectPrev(WPARAM lParam, LPARAM wParam);
	BOOL m_bFocus;
	CRavidTreeViewIPAddressEdit m_ctlEdit4;
	CRavidTreeViewIPAddressEdit m_ctlEdit3;
	CRavidTreeViewIPAddressEdit m_ctlEdit2;
	CRavidTreeViewIPAddressEdit m_ctlEdit1;
	//{{AFX_MSG(CRavidTreeViewItemIPAddress)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_ITEMIPADDRESS
