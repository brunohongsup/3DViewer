
#ifndef RT_RADIOBUTTON
#define RT_RADIOBUTTON

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewRadioButton.h : header file
//


// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"


// Radio Button Node
struct RT_RADIO_NODE
{
	CString m_strText;

	BOOL m_bChecked;

	CRect m_rcHitRect;

	RT_RADIO_NODE *m_nNextNode;
};


// Classes
class CRavidTreeView;

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewRadioButton window

class AFX_EXT_CLASS CRavidTreeViewRadioButton : public CWnd
{
// Construction
public:
	CRavidTreeViewRadioButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewRadioButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	int Node_GetChecked();
	void Node_SetChecked(int nIndex, bool bCheck);
	void Node_UnCheckAll();
	void SetRadioOptionsOwner(CRavidTreeView *otOption);
	RT_RADIO_NODE *Node_FindNode(CString strText);
	RT_RADIO_NODE *Node_FindNode(int nIndex);
	void Node_DeleteAll();
	void Node_Insert(CString strText, BOOL bChecked);
	virtual ~CRavidTreeViewRadioButton();

protected:
	

	// Generated message map functions
protected:
	RT_RADIO_NODE *m_nAllNodes;
	CRavidTreeView *m_rtRadioOption;
	//{{AFX_MSG(CRavidTreeViewRadioButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_RADIOBUTTON
