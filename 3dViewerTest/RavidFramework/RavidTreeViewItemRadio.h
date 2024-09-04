
#ifndef RT_ITEMRADIO
#define RT_ITEMRADIO

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewItemRadio.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"
#include "RavidTreeViewRadioButton.h"

// Classes
class CRavidTreeView;

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewItemRadio window

class AFX_EXT_CLASS CRavidTreeViewItemRadio : public CRavidTreeViewRadioButton, public CRavidTreeViewItem
{
// Construction
public:
	CRavidTreeViewItemRadio();
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
	//{{AFX_VIRTUAL(CRavidTreeViewItemRadio)
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetCheckedRadio();
	BOOL GetMultiline();
	void InsertNewRadio(CString strText, BOOL bChecked);
	BOOL CreateRadioItem();
	virtual ~CRavidTreeViewItemRadio();

protected:
	void ReCalculateHeight();

	// Generated message map functions
protected:
	BOOL m_bFocus;
	//{{AFX_MSG(CRavidTreeViewItemRadio)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_ITEMRADIO
