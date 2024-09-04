

#ifndef RT_TREEINFO
#define RT_TREEINFO

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewInfo.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewInfo window

// Classes
class CRavidTreeView;

class AFX_EXT_CLASS CRavidTreeViewInfo : public CStatic
{
// Construction
public:
	CRavidTreeViewInfo();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewInfo)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetOptionsOwner(CRavidTreeView *otOption);
	virtual ~CRavidTreeViewInfo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRavidTreeViewInfo)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CRavidTreeView *m_rtOption;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_TREEINFO
