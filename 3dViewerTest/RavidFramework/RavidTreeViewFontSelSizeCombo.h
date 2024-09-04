
#ifndef RT_FONTSEL_SIZECOMBO
#define RT_FONTSEL_SIZECOMBO

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontSelSizeCombo.h : header file
//
// Added Headers
#include "CommonRes.h"

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewFontSelSizeCombo window

class AFX_EXT_CLASS CRavidTreeViewFontSelSizeCombo : public CComboBox
{
// Construction
public:
	CRavidTreeViewFontSelSizeCombo();
	int AddSize(int nPointSize, long lfHeight);
	void GetPointSize(CY& cy);
	long GetHeight(int nSel = -1);
	void UpdateLogFont(LPLOGFONT lpLF, int nSel = -1);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewFontSelSizeCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRavidTreeViewFontSelSizeCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRavidTreeViewFontSelSizeCombo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_FONTSEL_SIZECOMBO
