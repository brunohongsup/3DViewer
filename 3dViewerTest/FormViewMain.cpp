#include "stdafx.h"
#include "resource.h"
#include "FormViewMain.h"
#include "EventHandlerMain.h"

IMPLEMENT_DYNCREATE(CFormViewMain, CRavidFormViewBase)

CFormViewMain::CFormViewMain()
	: CRavidFormViewBase(IDD_FORM_VIEW_MAIN)
{

}

CFormViewMain::~CFormViewMain()
{
}

void CFormViewMain::DoDataExchange(CDataExchange* pDX)
{
	CRavidFormViewBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormViewMain, CRavidFormViewBase)
	ON_BN_CLICKED(IDC_BUTTON1, &CFormViewMain::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFormViewMain::OnBnClickedButton2)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CFormViewMain::AssertValid() const
{
	CRavidFormViewBase::AssertValid();
}

#ifndef _WIN32_WCE
void CFormViewMain::Dump(CDumpContext& dc) const
{
	CRavidFormViewBase::Dump(dc);
}
#endif
#endif //_DEBUG


void CFormViewMain::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	do 
	{ 
		CEventHandlerMain* pHandler = dynamic_cast<CEventHandlerMain*>(CEventHandlerManager::GetEventHandler(0));
		if(!pHandler)
			break;

		pHandler->LoadTestFile();

	} while (false);
}


void CFormViewMain::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
