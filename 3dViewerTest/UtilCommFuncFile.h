#pragma once



#define PATH_ERROR			-1
#define PATH_NOT_FOUND		0
#define PATH_IS_FILE		1
#define PATH_IS_FOLDER		2

class CFException
{
public:

	CFException(DWORD dwErrCode);
	CFException(CString sErrText);
	CString GetErrorText() {return m_sError;}
	DWORD GetErrorCode() {return m_dwError;}

private:

	CString m_sError;
	DWORD m_dwError;
};


class CUtilCommFuncFile
{
public:
	CUtilCommFuncFile(void);
	virtual ~CUtilCommFuncFile(void);
	
	bool MakeFolder(LPCTSTR lpFolderName);
	bool FileDelete(LPCTSTR lpFileName);
	bool IsFileExist(LPCTSTR lpFileName);
	bool IsFolderExist(LPCTSTR lpFolderName);


	bool ReadInIString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpFileName, LPTSTR lpszBuf, int nSize, LPTSTR strInit = _T("0"));

	bool WriteInIString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName);

	void INI_GetInfoOfString(TCHAR *chBuf, CRavidPoint<int> *ptType);
	void INI_GetInfoOfString(TCHAR *chBuf, RECT *rtType);
	void INI_GetInfoOfString(TCHAR *chBuf, double *dType);
	void INI_GetInfoOfString(TCHAR *chBuf, CRavidPoint<double> *dptType);
	void INI_GetInfoOfString(TCHAR *chBuf, float *fType);
	void INI_GetInfoOfString(TCHAR *chBuf, long *lType);
	void INI_GetInfoOfString(TCHAR *chBuf, int *nType);
	void INI_GetInfoOfString(TCHAR *chBuf, bool *bType);
	void INI_GetInfoOfString(TCHAR *chBuf, long long *nType);

	void INI_SetInfoOfString(CString *strIni, CRavidPoint<int> ptType);
	void INI_SetInfoOfString(CString *strIni, RECT rtType);
	void INI_SetInfoOfString(CString *strIni, double dType);
	void INI_SetInfoOfString(CString *strIni, CRavidPoint<double> dptType);
	void INI_SetInfoOfString(CString *strIni, float fType);
	void INI_SetInfoOfString(CString *strIni, int nType);
	void INI_SetInfoOfString(CString *strIni, bool bType);


	void Initialize();

	void DoDelete(CString sPathName);
	void DoFolderCopy(CString sSourceFolder, CString sDestFolder, bool bDeleteAfterCopy = false);
	void DoCopy(CString sSource, CString sDest, bool bDeleteAfterCopy = false);
	int CheckPath(CString sPath);
	void PreparePath(CString &sPath);
	bool CanDelete(CString sPathName);
	void DoFileCopy(CString sSourceFile, CString sDestFile, bool bDeleteAfterCopy = false);
	void CheckSelfRecursion(CString sSource, CString sDest);
	CString ParseFolderName(CString sPathName);
	CString ChangeFileName(CString sFileName);

	bool MakeDirectory(CString strPathName);
	bool MakeDirectoryFullPath(CString strPathName);

	bool ReadTextFile(CString& filename, CString& contents);
	bool WriteTextFile(CString& filename, const CString& contents);
	bool AppendFile(CString& filename, const CString& contents);


protected:
	CString m_sError;
	DWORD	m_dwError;
	bool	m_bAskIfReadOnly;
	bool	m_bOverwriteMode;
	bool	m_bAborted;
	int		m_iRecursionLimit;

	
};

