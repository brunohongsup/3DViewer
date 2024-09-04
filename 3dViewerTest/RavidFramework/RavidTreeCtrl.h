#pragma once

class CRavidTreeCtrl : public CTreeCtrl
{
public:
	CRavidTreeCtrl();
	virtual ~CRavidTreeCtrl();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	DECLARE_MESSAGE_MAP()

public:
	std::vector<HTREEITEM> GetSelectedItems();

	virtual bool SelectItems(_In_ std::vector<HTREEITEM>& vctItems);

protected:

	virtual bool ClearSelection();

	virtual bool SelectItems(_In_ HTREEITEM hItemFrom, _In_ HTREEITEM hItemTo);

	virtual HTREEITEM GetFirstSelectedItem();
	virtual HTREEITEM GetNextSelectedItem(_In_ HTREEITEM hItem);
	virtual HTREEITEM GetPrevSelectedItem(_In_ HTREEITEM hItem);

	virtual bool SendSelChangedNotifyMessage();

	HTREEITEM m_hItemFirstSel = nullptr;
};