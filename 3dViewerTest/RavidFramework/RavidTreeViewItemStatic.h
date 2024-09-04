
#ifndef RT_ITEMSTATIC
#define RT_ITEMSTATIC

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"

// Classes
class CRavidTreeView;

class AFX_EXT_CLASS CRavidTreeViewItemStatic : public CRavidTreeViewItem
{
public:
	BOOL CreateStaticItem(DWORD dwOptions);
	BOOL GetOption(DWORD dwOption);
	void SetOption(DWORD dwOption, BOOL bSet);	
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);

	void SetStaticText(CString strStaticText);
	CString GetStaticText();
	CRavidTreeViewItemStatic();

	virtual ~CRavidTreeViewItemStatic();

protected:
	CString m_strStaticText;
	DWORD m_dwOptions;

};

#endif // !RT_ITEMSTATIC
