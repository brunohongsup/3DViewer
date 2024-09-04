
#ifndef RT_FILEDLG
#define RT_FILEDLG

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"

// Classes
class CRavidTreeView;

// Structures
struct RT_OPENFILENAMEEX : public OPENFILENAME 
{ 
	void*	m_pvReserved;
	DWORD	m_dwReserved;
	DWORD	m_dwFlagsEx;
};

class AFX_EXT_CLASS CRavidTreeViewFileDlg  
{
public:
	int SelectFolder(LPCTSTR lpszTitle = NULL, LPCTSTR lpszStartPath = NULL, UINT ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS, CWnd* pParentWnd = NULL);
	POSITION GetStartPosition() const;
	CString GetNextPathName(POSITION& pos) const;
	CString GetPathName() const;
	CString GetFileName() const;
	CString GetFileTitle() const;
	CString GetFileExt() const;
	CString GetFileDir() const;
	CString GetFileDrive() const;
	CString GetSelectedFolder() const;
	virtual int DoModal();
	void SetDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL, DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL);
	CRavidTreeViewFileDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL, DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL);
	CRavidTreeViewFileDlg();
	virtual ~CRavidTreeViewFileDlg();

protected:
	static int __stdcall BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

	RT_OPENFILENAMEEX	m_ofn;
	BOOL m_bOpenFileDialog;
	TCHAR m_szFile[MAX_PATH];
	TCHAR m_szFileTitle[MAX_PATH];
	TCHAR m_szSelectedFolder[MAX_PATH];
	CString m_strFilter;
};

#endif // !RT_FILEDLG
