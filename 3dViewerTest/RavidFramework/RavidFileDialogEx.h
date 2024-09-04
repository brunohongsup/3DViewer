#pragma once

#include "afxdlgs.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidFileDialogEx : public CFileDialog
		{
		public:
			CRavidFileDialogEx(_In_ BOOL bOpenFileDialog,	// TRUE for FileOpen, FALSE for FileSaveAs
							   _In_opt_ LPCTSTR lpszDefExt = nullptr,
							   _In_opt_ LPCTSTR lpszFileName = nullptr,
							   _In_opt_ DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
							   _In_opt_ LPCTSTR lpszFilter = nullptr,
							   _In_opt_ CWnd* pParentWnd = nullptr,
							   _In_opt_ DWORD dwSize = 0,
							   _In_opt_ BOOL bVistaStyle = true);

			~CRavidFileDialogEx();
		};
	}
}
