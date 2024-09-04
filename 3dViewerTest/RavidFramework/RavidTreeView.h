
#ifndef RT_OPTIONTREE
#define RT_OPTIONTREE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeView.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewInfo.h"
#include "RavidTreeViewList.h"
#include "RavidTreeViewItem.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItemEdit.h"
#include "RavidTreeViewItemStatic.h"
#include "RavidTreeViewItemComboBox.h"
#include "RavidTreeViewItemCheckBox.h"
#include "RavidTreeViewItemRadio.h"
#include "RavidTreeViewItemSpinner.h"
#include "RavidTreeViewItemColor.h"
#include "RavidTreeViewItemDate.h"
#include "RavidTreeViewItemImage.h"
#include "RavidTreeViewItemFont.h"
#include "RavidTreeViewItemFile.h"
#include "RavidTreeViewItemFile.h"
#include "RavidTreeViewItemIPAddress.h"
#include "RavidTreeViewItemHyperLink.h"
#include "RavidTreeViewItemButton.h"

#include <vector>


// Definitions
typedef BOOL (CALLBACK* ENUM_OPTIONITEMPROC)(CRavidTreeView* otProp, CRavidTreeViewItem* otiItem, LPARAM lParam);
// CPropTree WM_NOTIFY notification structure
typedef struct _NMOPTIONTREE
{
    NMHDR hDR;
	CRavidTreeViewItem* otiItem;
} NMOPTIONTREE, *PNMOPTIONTREE, FAR *LPNMOPTIONTREE;


// Global Functions
// -- Draw dark horizontal line
static void _DrawDarkHLine(HDC hdc, long lX, long lY, long lWidth)
{
	CRect rcPaint(lX, lY, lX + lWidth, lY + 1);
	int nOldBack = SetBkColor(hdc, GetSysColor(COLOR_BTNSHADOW));

	// GDI calls driver directly
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, rcPaint, 0, 0, 0); 

	// Restore
	SetBkColor(hdc, nOldBack);	
}
// -- Draw dark vertical line
static void _DrawDarkVLine(HDC hdc, long lX, long lY, long lHeight)
{
	CRect rcPaint(lX, lY, lX + 1, lY + lHeight);
	int nOldBack = SetBkColor(hdc, GetSysColor(COLOR_BTNSHADOW));

	// GDI calls driver directly
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, rcPaint, 0, 0, 0); 

	// Restore
	SetBkColor(hdc, nOldBack);	
}
// -- Draw lite horizontal line
static void _DrawLiteHLine(HDC hdc, long lX, long lY, long lWidth)
{
	HBRUSH hbr = (HBRUSH )CDC::GetHalftoneBrush()->GetSafeHandle();
	SetBrushOrgEx(hdc, 0, 0, NULL);
	UnrealizeObject(hbr);
	HBRUSH holdbr = (HBRUSH )SelectObject(hdc, hbr);
	COLORREF rcOldColor = SetTextColor(hdc, GetSysColor(COLOR_BTNSHADOW));
	int nOldBack = SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
	PatBlt(hdc, lX, lY, lWidth, 1, PATCOPY);
	SelectObject(hdc, holdbr);
	SetTextColor(hdc, rcOldColor);
	SetBkColor(hdc, nOldBack);
}
// -- Draw lite vertical line
static void _DrawLiteVLine(HDC hdc, long lX, long lY, long lHeight)
{
	HBRUSH hbr = (HBRUSH )CDC::GetHalftoneBrush()->GetSafeHandle();
	SetBrushOrgEx(hdc, 0, 0, NULL);
	UnrealizeObject(hbr);
	HBRUSH holdbr = (HBRUSH )SelectObject(hdc, hbr);
	COLORREF rcOldColor = SetTextColor(hdc, GetSysColor(COLOR_BTNSHADOW));
	int nOldBack = SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
	PatBlt(hdc, lX, lY, 1, lHeight, PATCOPY);
	SelectObject(hdc, holdbr);
	SetTextColor(hdc, rcOldColor);
	SetBkColor(hdc, nOldBack);
}
// -- Draw selection rectangle
static void _DrawSelectRect(HDC hdc, long lX, long lY, long lWidth)
{
	HBRUSH hbr = (HBRUSH )CDC::GetHalftoneBrush()->GetSafeHandle();
	SetBrushOrgEx(hdc, 0, 0, NULL);
	UnrealizeObject(hbr);
	HBRUSH holdbr = (HBRUSH )SelectObject(hdc, hbr);
	COLORREF rcOldColor = SetTextColor(hdc, GetSysColor(COLOR_3DHILIGHT));
	int nOldBack = SetBkColor(hdc, GetSysColor(COLOR_3DFACE));
	PatBlt(hdc, lX, lY, lWidth, 1, PATCOPY);
	SelectObject(hdc, holdbr);
	SetTextColor(hdc, rcOldColor);
	SetBkColor(hdc, nOldBack);
}
// -- Draw splitter bar selected
static void _DrawXorBar(HDC hdc, int x1, int y1, int nWidth, int nHeight)
{
	// Declare variables
	static WORD _dotPatternBmp[8] = { 0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055};
	HBITMAP hbm;
	HBRUSH  hbr, hbrushOld;

	// Create a monochrome checkered pattern
	hbm = CreateBitmap(8, 8, 1, 1, _dotPatternBmp);

	hbr = CreatePatternBrush(hbm);
    
	SetBrushOrgEx(hdc, x1, y1, 0);
	hbrushOld = (HBRUSH)SelectObject(hdc, hbr);
    
	// Draw the checkered rectangle to the screen
	PatBlt(hdc, x1, y1, nWidth, nHeight, PATINVERT);
    
	SelectObject(hdc, hbrushOld);
    
	DeleteObject(hbr);
	DeleteObject(hbm);
}
// -- Make short string
static LPCTSTR _MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nWidth, int nOffset)
{
	// Declare variables
	const _TCHAR szThreeDots[]=_T("...");
	int nStringLen = lstrlen(lpszLong);
	static _TCHAR szShort[MAX_PATH];
	int nAddLen;

	// Validate length
	if(nStringLen == 0 || pDC->GetTextExtent(lpszLong,nStringLen).cx + nOffset <= nWidth)
	{
		return lpszLong;
	}

	// Shorten
	lstrcpy(szShort, lpszLong);
	nAddLen = pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;
	for(int i = nStringLen - 1; i > 0; i--)
	{
		szShort[i] = 0;
		if(pDC->GetTextExtent(szShort, i).cx + nOffset + nAddLen <= nWidth)
		{
			break;
		}
	}
	lstrcat(szShort, szThreeDots);

	return szShort;
}

// Classes
class CRavidTreeView;

/////////////////////////////////////////////////////////////////////////////
// CRavidTreeView window

class AFX_EXT_CLASS CRavidTreeView : public CWnd
{
// Construction
	DECLARE_DYNCREATE(CRavidTreeView)
public:
	CRavidTreeView();
	virtual ~CRavidTreeView();
	BOOL Create(DWORD dwStyle, RECT rcRect, CWnd* pParentWnd, DWORD dwTreeOptions, UINT nID);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRavidTreeView)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Expand(CRavidTreeViewItem *pItem, BOOL bExpand);
	void ExpandAllItems();
	void ShadeRootItems(BOOL bShade);
	BOOL GetShadeRootItems();
	void ShadeExpandColumn(BOOL bShade);
	BOOL GetShadeExpandColumn();
	void ClearAllLabelRect();
	CRect GetLargestVisibleLabel();
	void SetNotify(BOOL bNotify, CWnd *pWnd);
	BOOL GetNotify();
	void SetDefInfoTextNoSel(BOOL bNoSelect);
	BOOL GetDefInfoTextNoSel();
	DWORD GetTreeOptions();
	
	void SetSingleSelection(_In_ bool bSingle);
	BOOL IsSingleSelection();

	CRavidTreeViewItem * FocusNext();
	CRavidTreeViewItem * FocusPrev();
	CRavidTreeViewItem * FocusLast();
	BOOL IsDisableInput();
	CRavidTreeViewItem * InsertItem(CRavidTreeViewItem* otiItem, CRavidTreeViewItem* otiParent = NULL, bool bUpdate = true);
	void ClearVisibleList();
	void SetColumn(long lColumn);
	long GetColumn();
	void DeleteItem(CRavidTreeViewItem* otiItem, bool bUpdate = true);
	void DeleteAllItems(bool bUpdate = true);
	void SetOriginOffset(long lOffset);
	BOOL IsItemVisible(CRavidTreeViewItem* otiItem);
	void UpdatedItems();
	void RefreshItems(CRavidTreeViewItem* otiItem = NULL);
	void UpdateMoveAllItems();
	void EnsureVisible(CRavidTreeViewItem* otiItem);
	void SetFocusedItem(CRavidTreeViewItem* otiItem);
	CRavidTreeViewItem * FocusFirst();
	void SelectItems(CRavidTreeViewItem* otiItem, BOOL bSelect);
	void DisableInput(BOOL bDisable = TRUE);
	CRavidTreeViewItem * FindItem(const POINT& pt);
	CRavidTreeViewItem * FindItem(UINT uCtrlID);
	long HitTest(const POINT& pt);
	void AddToVisibleList(CRavidTreeViewItem* otiItem);
	CRavidTreeViewItem * GetVisibleList();
	CRavidTreeViewItem * GetFocusedItem();
	CRavidTreeViewItem * GetRootItem();
	BOOL GetShowInfoWindow();
	void ShowInfoWindow(BOOL bShow);
	CFont* GetNormalFont();
	CFont* GetBoldFont();
	const POINT& GetOrigin();
	CWnd* GetCtrlParent();
	LRESULT SendNotify(UINT uNotifyCode, CRavidTreeViewItem* otiItem = NULL);
	
	std::vector<CRavidTreeViewItem*> GetAllItems();
	std::vector<CRavidTreeViewItem*> GetVisibledItems();
	std::vector<CRavidTreeViewItem*> GetSelectedItems();

	// Generated message map functions
protected:
	BOOL EnumItems(CRavidTreeViewItem* otiItem, ENUM_OPTIONITEMPROC enumProc, LPARAM lParam = 0L);
	static BOOL CALLBACK EnumFindItem(CRavidTreeView* otProp, CRavidTreeViewItem* otiItem, LPARAM lParam);
	static BOOL CALLBACK EnumMoveAll(CRavidTreeView* otProp, CRavidTreeViewItem* otiItem, LPARAM lParam);
	static BOOL CALLBACK EnumNotifyExpand(CRavidTreeView* otProp, CRavidTreeViewItem* otiItem, LPARAM lParam);	
	static BOOL CALLBACK EnumRefreshAll(CRavidTreeView* otProp, CRavidTreeViewItem* otiItem, LPARAM lParam);
	static BOOL CALLBACK EnumExpandAll(CRavidTreeView* otProp, CRavidTreeViewItem* otiItem, LPARAM lParam);
	static BOOL CALLBACK EnumSelectAll(CRavidTreeView* otProp, CRavidTreeViewItem* otiItem, LPARAM lParam);
	static BOOL CALLBACK EnumGetLargestVisibleLabelRect(CRavidTreeView* otProp, CRavidTreeViewItem* otiItem, LPARAM lParam);
	static BOOL CALLBACK EnumClearAllLabelRect(CRavidTreeView* otProp, CRavidTreeViewItem* otiItem, LPARAM lParam);
	void DeleteGlobalResources();
	void InitGlobalResources();
	void ResizeAllWindows(int cx, int cy);
	void Delete(CRavidTreeViewItem* otiItem, bool bUpdate = true);
	//{{AFX_MSG(CRavidTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Variables
protected:
	CFont* m_fNormalFont = nullptr;
	CFont* m_fBoldFont = nullptr;
	static UINT	m_uInstanceCount;
	static CRavidTreeViewItem *m_rtiFound;
	DWORD m_dwTreeOptions;
	CRavidTreeViewInfo m_rtInfo;
	CRavidTreeViewItem	m_rtiRoot;
	CRavidTreeViewItem* m_rtiVisibleList;
	CRavidTreeViewItem* m_rtiFocus;
	CPoint m_ptOrigin;
	CRavidTreeViewList m_rtlList;
	BOOL m_bDisableInput;
	UINT m_uLastUID;
	static CRect m_rcLargestLabel;
	CWnd *m_pNotify;

	bool m_bIsSingleSelection = true;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !RT_OPTIONTREE


