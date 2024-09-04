
#ifndef RT_ITEMFILE
#define RT_ITEMFILE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RavidTreeViewItemFile.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "RavidTreeViewDef.h"
#include "RavidTreeViewItem.h"
#include "RavidTreeViewFileDlg.h"

class AFX_EXT_CLASS CRavidTreeViewItemFile : public CRavidTreeViewItem
{
public:
	CRavidTreeViewItemFile();
	virtual ~CRavidTreeViewItemFile();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);

	CString GetSelectedFolder();
	CString GetFileDrive();
	CString GetFileDir();
	CString GetFileExt();
	CString GetFileTitle();
	CString GetFileName();
	CString GetPathName();
	CString GetNextPathName(POSITION& pos);
	POSITION GetStartPosition();
	CString GetDialogTitle();
	void SetDialogTitle(CString strTitle);
	CString GetFilter();
	void SetFilter(CString strFilter);
	CString GetDefaultExtention();
	void SertDefaultExtention(CString strExt);
	DWORD GetDialogFlags();
	void SetDialogFlags(DWORD dwFlags);
	void AddFileName(CString strFile);
	BOOL CreateFileItem(CString strFile, CString strDefExt, CString strFilter, DWORD dwOptions, DWORD dwDlgFlags);

protected:
	CString GetFileDrive(CString strFile);
	CString GetFileDirectory(CString strFile);
	CString GetFileExtention(CString strFile);
	BOOL GetOption(DWORD dwOption);
	void SetOption(DWORD dwOption, BOOL bSet);	
	BOOL GetDialogFlag(DWORD dwOption);
	void SetDialogFlag(DWORD dwOption, BOOL bSet);	
	DWORD m_dwOptions;
	DWORD m_dwDlgFlags;
	long m_lDefaultHeight;
	CRavidTreeViewFileDlg m_dlgFile;
	CStringArray m_strFileNames;
	CString m_strDefExt;
	CString m_strFilter;
	CString m_strDlgTitle;
};

#endif // !RT_ITEMFILE
