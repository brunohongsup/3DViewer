#pragma once

#include "RavidObjectEx.h"

namespace Ravid
{
	namespace Framework
	{
		class CRavidMainFrame;

		class AFX_EXT_CLASS CRavidMainFrameToolBarHandler : public CRavidObjectEx
		{
			RavidUseDynamicCreation();

		public:
			CRavidMainFrameToolBarHandler();
			virtual ~CRavidMainFrameToolBarHandler();

			bool OpenModelManager();
			bool OpenDeviceManager();
			bool LogIn();
			bool LogOut();
			bool OpenAuthorityManager();
			bool SetScreenSaver();
			bool StartSequence();
			bool PauseSequence();
			bool StopSequence();
			bool RunUIConfiguration(_In_ long nNumber);
			bool LanguageChange(_In_ long nNumber);
			bool LanguageChange(_In_ CString strLanguage);
			bool OpenLogViewer();
			bool CloseLogViewer();
			bool OpenImageProcessingTool();
			bool CloseImageProcessingTool();

		private:
			CRavidMainFrame* GetMainFrame();
		};
	}
}

