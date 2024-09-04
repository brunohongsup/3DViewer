
#ifndef RT_ITEMCOLOR
#define RT_ITEMCOLOR

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewItemColor.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"
#include "RavidTreeViewColorPopUp.h"

// Classes
class CRavidTreeView;

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewItemColor window

class AFX_EXT_CLASS CRavidTreeViewItemColor : public CWnd, public CRavidTreeViewItem
{
// Construction
public:
	CRavidTreeViewItemColor();
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
	//{{AFX_VIRTUAL(CRavidTreeViewItemColor)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL GetOption(DWORD dwOption);
	void SetOption(DWORD dwOption, BOOL bSet);
	COLORREF GetAutomaticColor();
	void SetAutomaticColor(COLORREF crAutomatic);
	BOOL CreateColorItem(DWORD dwOptions, COLORREF rcColor, COLORREF rcAutomatic);
	void SetColor(COLORREF rcColor);
	COLORREF GetColor();
	virtual ~CRavidTreeViewItemColor();

	// Generated message map functions
protected:
	void DrawControl(CDC *pDC, const RECT &rcRect);
	BOOL m_bFocus;
	COLORREF m_crColor;
	COLORREF m_crAutomatic;
	DWORD m_dwOptions;
	//{{AFX_MSG(CRavidTreeViewItemColor)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnSelEndOK(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSelEndCancel(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSelChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCloseColorPopUp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_ITEMCOLOR
