
#ifndef RT_IMAGEPOPUP
#define RT_IMAGEPOPUP

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewImagePopUp.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"
#include <afxtempl.h>

// Classes
class CRavidTreeView;

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewImagePopUp window

class AFX_EXT_CLASS CRavidTreeViewImagePopUp : public CWnd
{
// Construction
public:
	CRavidTreeViewImagePopUp(CPoint pPoint, CSize sImageSize, int nNumColumns, CImageList *imgImageList, CRavidTreeView *otImageOption, CStringArray &strText, int nSelected, CWnd* pParentWnd);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewImagePopUp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRavidTreeViewImagePopUp();

	// Generated message map functions
protected:
	void SetUpImages();
	void SetUpToolTips();
	void SetWindowSize();
	BOOL Create(CPoint pPoint);
	CToolTipCtrl m_ttToolTip;
	CPoint m_pHover;
	CRavidTreeView *m_rtImageOption;
	CImageList *m_pImageList;
	CWnd *m_pParent;
	CSize m_sImageSizes;
	int m_nNumColumns;
	int m_nNumRows;
	CStringArray m_strText;
	CArray<CRect, CRect&> m_rcRectangles;
	int m_nSelected;
	//{{AFX_MSG(CRavidTreeViewImagePopUp)
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcDestroy();
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_IMAGEPOPUP
