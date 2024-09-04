

#ifndef RT_ITEMIMAGE
#define RT_ITEMIMAGE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewItemImage.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"
#include "RavidTreeViewImagePopUp.h"

// Classes
class CRavidTreeView;

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeViewItemImage window

class AFX_EXT_CLASS CRavidTreeViewItemImage : public CWnd, public CRavidTreeViewItem
{
// Construction
public:
	CRavidTreeViewItemImage();
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
	//{{AFX_VIRTUAL(CRavidTreeViewItemImage)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSelection(int nSelection);
	int GetSelection();
	void AddIcon(CString strIcon, CString strText);
	void AddBitmap(CString strBitmap, COLORREF crMask, CString strText);
	void AddBitmap(UINT uBitmap, COLORREF crMask, CString strText);
	void AddBitmap(CBitmap &bBitmap, COLORREF crMask, CString strText);
	void AddIcon(UINT uIcon, CString strText);
	void AddIcon(HICON hIcon, CString strText);
	BOOL GetOption(DWORD dwOption);
	void SetOption(DWORD dwOption, BOOL bSet);
	void SetNumberColumns(int nNum);
	CSize GetImageSize();
	void SetImageSize(CSize sSize);
	BOOL CreateImageItem(DWORD dwOptions, CSize sImageSizes, int nNumberColumns);
	virtual ~CRavidTreeViewItemImage();

	// Generated message map functions
protected:
	HICON CreateGreyScaleIcon(HICON hIcon);
	void DitherBlt(CDC *drawdc, int nXDest, int nYDest, int nWidth, int nHeight, CBitmap &bmp, int nXSrc, int nYSrc);
	BOOL GetBitmapFromImageList(CDC *pDC, HICON hIcon, CBitmap &bmp);
	void DrawControl(CDC *pDC, const RECT &rcRect);
	BOOL m_bFocus;
	int m_nNumColumns;
	CSize m_sImageSizes;
	int m_nSelected;
	CImageList m_imgImages;
	DWORD m_dwOptions;
	CStringArray m_strText;
	//{{AFX_MSG(CRavidTreeViewItemImage)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg LRESULT OnPopUpClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_ITEMIMAGE
