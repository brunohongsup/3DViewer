
#ifndef RT_SPINNERBUTTON
#define RT_SPINNERBUTTON

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewSpinnerButton.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"
#include "RavidTreeViewSpinnerEdit.h"

// Classes
class CRavidTreeView;

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewSpinnerButton window

class AFX_EXT_CLASS CRavidTreeViewSpinnerButton : public CWnd
{
// Construction
public:
	CRavidTreeViewSpinnerButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewSpinnerButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL GetOption(DWORD dwOption);
	void SetOption(DWORD dwOption, BOOL bSet);	
	CString GetEditText();
	BOOL IsStringNumeric(CString strString);
	CEdit* GetEdit();
	void ResizeEdit();
	BOOL GetEditDword(DWORD &dwReturn);
	BOOL GetEditInt(int &nReturn);
	BOOL GetEditLong(long &lReturn);
	BOOL GetEditDouble(double &dReturn);
	BOOL GetEditFloat(float &fReturn);
	void SetEditLong(long lValue);
	void SetEditDword(DWORD dwValue);
	void SetEditFloat(float fValue);
	void SetEditInt(int nValue);
	void SetEditDouble(double dValue);
	void GetRange(double &dBottom, double &dTop);
	void SetRange(double dBottom, double dTop);
	void SetSpinnerOptionsOwner(CRavidTreeView *otOption);
	virtual ~CRavidTreeViewSpinnerButton();

	virtual void ChangeValue();

protected:
	LRESULT WM_ForceRedraw(WPARAM wParam, LPARAM lParam);
	LRESULT WM_EditUp(WPARAM wParam, LPARAM lParam);
	LRESULT WM_EditDown(WPARAM wParam, LPARAM lParam);	
	double _GetValue();
	
	// Generated message map functions
protected:
	void RepeatButton();
	CRavidTreeView *m_rtSpinnerOption;
	double m_dRangeTop;
	double m_dRangeBottom;
	CRect m_rcButtonTop;
	CRect m_rcButtonBottom;
	BOOL m_bBottomPressed;
	BOOL m_bTopPressed;
	CPoint m_ptSavePoint;
	BOOL m_bFirstRepeat;
	CRavidTreeViewSpinnerEdit m_ctlEdit;
	int m_nRepeatDelay;
	int m_nRepeatRate;
	DWORD m_dwOptions;
	//{{AFX_MSG(CRavidTreeViewSpinnerButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_SPINNERBUTTON
