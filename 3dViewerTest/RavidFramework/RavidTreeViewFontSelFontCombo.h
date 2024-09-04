
#ifndef RT_FONTSEL_FONTCOMBO
#define RT_FONTSEL_FONTCOMBO

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontSelFontCombo.h : header file
//

// Added Headers
#include "CommonRes.h"

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewFontSelFontCombo window

// Structures
struct RT_FONTSEL_ITEM
{
	DWORD dwFontType;
	LOGFONT lf;
};

class AFX_EXT_CLASS CRavidTreeViewFontSelFontCombo : public CComboBox
{
// Construction
public:
	CRavidTreeViewFontSelFontCombo();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeViewFontSelFontCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDIS);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRavidTreeViewFontSelFontCombo();
	int AddFont(LOGFONT *pLF, DWORD FontType);
	CString GetCurrentName();

	RT_FONTSEL_ITEM* GetFontItem(int nSel = -1);
	LPLOGFONT GetLogFont(int nSel = -1);
	DWORD GetFontType(int nSel = -1);
	// Generated message map functions
protected:
	//{{AFX_MSG(CRavidTreeViewFontSelFontCombo)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	BOOL SelectFontName(CString strName);


protected:
	CBitmap m_bmpTrueType;
	CBitmap m_bmpMask;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_FONTSEL_FONTCOMBO
