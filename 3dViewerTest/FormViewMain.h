#pragma once

using namespace Ravid::Framework;

class CFormViewMain : public CRavidFormViewBase
{
	DECLARE_DYNCREATE(CFormViewMain)

protected:
	CFormViewMain();
	virtual ~CFormViewMain();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM_VIEW_MAIN };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
