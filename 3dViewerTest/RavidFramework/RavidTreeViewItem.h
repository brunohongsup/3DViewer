
#ifndef RT_TREEITEM
#define RT_TREEITEM

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"

// Classes
class CRavidTreeView;

class AFX_EXT_CLASS CRavidTreeViewItem  
{
public:
	COLORREF GetTextColor();
	void SetTextColor(COLORREF crColor);
	COLORREF GetLabelBackgroundColor();
	void SetLabelBackgroundColor(COLORREF crColor);
	COLORREF GetRootBackgroundColor();
	void SetRootBackgroundColor(COLORREF crColor);
	COLORREF GetLabelTextColor();
	void SetLabelTextColor(COLORREF crColor);
	COLORREF GetBackgroundColor();
	void SetBackgroundColor(COLORREF crColor);
	BOOL GetDrawMultiline();
	void SetDrawMultiline(BOOL bMultiline);
	int GetItemType();
	void SetItemType(int nType);
	void SetLabelRect(CRect rcLabel);
	CRect GetLabelRect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);
	virtual void OnActivate();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void OnExpand(BOOL bExpand);


	void SetItemHeight(long lHeight);
	void CommitChanges();
	void Activate();
	long GetHeight();
	virtual long DrawItem(CDC* pDC, const RECT &rcRect, long x, long y);
	void SetOptionsOwner(CRavidTreeView* otOption);
	long GetTotalHeight();
	BOOL IsRootLevel();
	BOOL HitExpand(const POINT& pt);
	void ReadOnly(BOOL bReadOnly = TRUE);
	void Expand(BOOL bExpand = TRUE);
	void Select(BOOL bSelect = TRUE);
	BOOL IsActivated();
	BOOL IsReadOnly();
	BOOL IsSelected();
	BOOL IsExpanded();
	void SetNextVisible(CRavidTreeViewItem *otiNextVisible);
	CRavidTreeViewItem * GetNextVisible();
	void SetChild(CRavidTreeViewItem *otiChild);
	CRavidTreeViewItem * GetChild();
	void SetSibling(CRavidTreeViewItem *otiSibling);
	CRavidTreeViewItem * GetSibling();
	void SetParent(CRavidTreeViewItem *otiParent);
	CRavidTreeViewItem * GetParent();
	UINT GetCtrlID();
	void SetCtrlID(UINT nID);
	CString GetLabelText();
	void SetLabelText(CString strLabel);
	CString GetInfoText();
	void SetInfoText(CString strText);
	void SetUserParam(LPARAM lParam);
	LPARAM GetUserParam();
	const POINT& GetLocation();
	CRavidTreeViewItem();
	virtual ~CRavidTreeViewItem();

	virtual bool SelectNext();
	virtual bool SelectPrev();

	virtual void ActivateCurrent();

protected:
	void _DrawExpand(HDC hdc, long lX, long lY, BOOL bExpand, BOOL bFill);
	BOOL IsStringEmpty(CString strString);

protected:
	CString m_strLabel;
	CString m_strInfoText;
	UINT m_uControlID;
	CRavidTreeViewItem *m_rtiParent;
	CRavidTreeViewItem *m_rtiSibling;
	CRavidTreeViewItem *m_rtiChild;
	CRavidTreeViewItem *m_rtiNextVisible;
	CRect m_rcExpand;
	CRavidTreeView *m_rtOption;
	CPoint m_ptLocation;
	CRect m_rcAttribute;
	BOOL m_bCommitOnce;
	LPARAM m_lParam;
	long m_lItemHeight;
	CRect m_rcLabelRect;
	BOOL m_bSelected;
	BOOL m_bExpanded;
	BOOL m_bActivated;
	BOOL m_bReadOnly;
	int m_nItemType;
	BOOL m_bDrawMultiline;
	COLORREF m_crBackground;
	COLORREF m_crText;
	COLORREF m_crRootBackground;
	COLORREF m_crLabelText;
	COLORREF m_crLabelBackground;

	LPARAM m_lParamUser = 0;
};

#endif // !RT_TREEITEM
