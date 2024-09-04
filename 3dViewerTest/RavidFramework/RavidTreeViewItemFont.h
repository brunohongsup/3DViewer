
#ifndef RT_ITEMFONT
#define RT_ITEMFONT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewItemFont.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"
#include "RavidTreeViewFontSel.h"

class AFX_EXT_CLASS CRavidTreeViewItemFont : public CRavidTreeViewItem
{
public:
	CRavidTreeViewItemFont();
	void SetOption(DWORD dwOption, BOOL bSet);
	BOOL GetOption(DWORD dwOption);
	void SetApplyWindow(CWnd *pWnd);
	void SetCustomSample(CString strSample);
	COLORREF GetDefaultTextColor();
	COLORREF GetTextColor();
	void SetTextColor(COLORREF crColor);
	void SetDefaultTextColor(COLORREF crColor);
	void SetDefaultFont(CHARFORMAT cfFont);
	void SetCurFont(CHARFORMAT cfFont);
	void SetDefaultFont(LOGFONT lf);
	void SetCurFont(LOGFONT lf);
	BOOL CreateFontItem(LOGFONT lfFont, COLORREF crFontColor, LOGFONT lfDefaultFont, COLORREF crDefaultFontColor, DWORD dwOptions);
	BOOL CreateFontItem(LOGFONT lfFont, COLORREF crFontColor, DWORD dwOptions);
	BOOL CreateFontItem(CHARFORMAT cfFont, COLORREF crFontColor, CHARFORMAT cfDefaultFont, COLORREF crDefaultFontColor, DWORD dwOptions);
	BOOL CreateFontItem(CHARFORMAT cfFont, COLORREF crFontColor, DWORD dwOptions);
	virtual ~CRavidTreeViewItemFont();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);

protected:
	void ConvertFont(LOGFONT &lf, CHARFORMAT &cfFont);
	void ConvertFont(CHARFORMAT &cfFont, LOGFONT &lf);
	COLORREF m_crDefTextColor;
	COLORREF m_crTextColor;
	LOGFONT m_lfFont;
	LOGFONT m_lfDefaultFont;
	DWORD m_dwOptions;
	CString m_strCustomSample;
	CWnd *m_pApplyWindow;
	long m_lDefaultHeight;

};

#endif // !RT_ITEMFONT
