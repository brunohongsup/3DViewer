
#include "CommonRes.h"


#ifndef RT_FONTSEL_COLORBUTTON
#define RT_FONTSEL_COLORBUTTON
#pragma once

#ifndef RT_COLORPOPUP
	#include "RavidTreeViewColorPopUp.h"
#endif

void AFXAPI DDX_FontSelColorButton(CDataExchange *pDX, int nIDC, COLORREF& crColor);

class AFX_EXT_CLASS CRavidTreeViewFontSelColorButton : public CButton
{
public:
	DECLARE_DYNCREATE(CRavidTreeViewFontSelColorButton);

	CRavidTreeViewFontSelColorButton();
	virtual ~CRavidTreeViewFontSelColorButton();	
	COLORREF GetColor() const;
	void SetColor(COLORREF crColor);
	COLORREF GetDefaultColor() const;
	void SetDefaultColor(COLORREF crColor);
	void SetCustomText(LPCTSTR tszText);
	void SetDefaultText(LPCTSTR tszText);
	void SetTrackSelection(BOOL bTrack);
	BOOL GetTrackSelection() const;

	//{{AFX_VIRTUAL(CRavidTreeViewFontSelColorButton)
    public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CRavidTreeViewFontSelColorButton)
    afx_msg BOOL OnClicked();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
	afx_msg LRESULT OnSelEndOK(WPARAM lParam, LPARAM wParam);
    afx_msg LRESULT OnSelEndCancel(WPARAM lParam, LPARAM wParam);
    afx_msg LRESULT OnSelChange(WPARAM lParam, LPARAM wParam);
	static void DrawArrow(CDC* pDC, RECT* pRect, int nDirection = 0, COLORREF crArrow = RGB(0, 0, 0));


	DECLARE_MESSAGE_MAP()

	COLORREF m_crColor;
	COLORREF m_crDefaultColor;
	CString m_strDefaultText;
	CString m_strCustomText;
	BOOL m_bPopupActive;
	BOOL m_bTrackSelection;

private:

	typedef CButton _Inherited;

};

#endif //!RT_FONTSEL_COLORBUTTON

