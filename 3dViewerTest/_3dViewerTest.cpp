#include "stdafx.h"
#include "_3dViewerTest.h"
#include "FormViewMain.h"
#include "EventHandlerMain.h"
#include "SequenceMain.h"

#include "ImageView3D.h"

using namespace Ravid::Framework;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(C_3dViewerTest, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinAppEx::OnHelp)
END_MESSAGE_MAP()

C_3dViewerTest::C_3dViewerTest()
{
}

C_3dViewerTest theApp;

BOOL C_3dViewerTest::InitInstance()
{
	CWinAppEx::InitInstance();

	SetRegistryKey(_T("_3dViewerTest"));

	CUIManager::AddEasyConfigurationView(RUNTIME_CLASS(CFormViewMain));
	CUIManager::AddEasyConfigurationView(RUNTIME_CLASS(CImageView3D));

	bool bSettingMode = false;
	CFrameworkManager::Begin(_T("Settings"), bSettingMode);

	CUIManager::RunUIConfiguration(0);

	CEventHandlerMain* pEventHandler = new CEventHandlerMain;
	if(pEventHandler)
		CEventHandlerManager::AddEventHandler(pEventHandler);

	CSequenceMain* pSequence = new CSequenceMain;
	if(pSequence)
		CSequenceManager::RegisterSequence(pSequence);

	return true;
}

