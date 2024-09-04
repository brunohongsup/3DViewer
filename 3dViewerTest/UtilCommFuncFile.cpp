#include "stdafx.h"
#include "UtilCommFuncFile.h"



CUtilCommFuncFile::CUtilCommFuncFile(void)
{
	Initialize();
}


CUtilCommFuncFile::~CUtilCommFuncFile(void)
{
}

//************************************************************************************************************
CFException::CFException(DWORD dwErrCode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			      NULL, dwErrCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	m_sError = (LPTSTR)lpMsgBuf;
	LocalFree(lpMsgBuf);
	m_dwError = dwErrCode;
}


CFException::CFException(CString sErrText)
{
	m_sError = sErrText;
	m_dwError = 0;
}

void CUtilCommFuncFile::Initialize()
{
	m_sError = _T("No error");
	m_dwError = 0;
	m_bAskIfReadOnly = true;
	m_bOverwriteMode = true;
	m_bAborted = false;
	m_iRecursionLimit = -1;
}




bool CUtilCommFuncFile::FileDelete(LPCTSTR lpFileName)
{
	return DeleteFile(lpFileName);
}


bool CUtilCommFuncFile::MakeFolder(LPCTSTR lpFolderName)
{
	return CreateDirectory(lpFolderName, NULL);

}


bool CUtilCommFuncFile::IsFileExist(LPCTSTR lpFileName)
{
	HANDLE hFile;
	hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
		return false;

	CloseHandle(hFile);

	return true;
}

bool CUtilCommFuncFile::IsFolderExist(LPCTSTR lpFolderName)
{
	DWORD dwAttr = GetFileAttributes(lpFolderName);
	if (dwAttr == 0xffffffff) 
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND) 
			return false;	//Not Found

		return false;	//Path Error
	}

	if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) 
		return true;

	return true;
}


bool CUtilCommFuncFile::ReadInIString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpFileName, LPTSTR lpszBuf, int nSize, LPTSTR strInit/* = _T("0")*/)
{
	return GetPrivateProfileString(lpAppName, lpKeyName, strInit, lpszBuf, nSize, lpFileName);
}

bool CUtilCommFuncFile::WriteInIString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName)
{
	return WritePrivateProfileString(lpAppName, lpKeyName, lpString, lpFileName);
}

void CUtilCommFuncFile::INI_GetInfoOfString(TCHAR *chBuf, CRavidPoint<int> *ptType)
{
	int     nPosPre, nPosNext;
	POINT   Pt;
	CString str;
	str.Format(_T("%s"),chBuf);

	nPosPre  = 1;
	nPosNext = str.Find(_T(","),nPosPre);
	Pt.x     = _tstoi(str.Mid(nPosPre,nPosNext-nPosPre));

	nPosPre  = nPosNext +1;
	nPosNext = str.Find(_T(")"),nPosPre);
	Pt.y     = _tstoi(str.Mid(nPosPre,nPosNext-nPosPre));

	*ptType = Pt;
}


void CUtilCommFuncFile::INI_GetInfoOfString(TCHAR *chBuf, RECT *rtType)
{
	int     nPosPre, nPosNext;
	RECT    Rect;
	CString str;
	str.Format(_T("%s"),chBuf);

	nPosPre     = 1;
	nPosNext    = str.Find(_T(","),nPosPre);
	Rect.left   = _tstoi(str.Mid(nPosPre,nPosNext-nPosPre));

	nPosPre     = nPosNext +1;
	nPosNext    = str.Find(_T(","),nPosPre);
	Rect.top    = _tstoi(str.Mid(nPosPre,nPosNext-nPosPre));

	nPosPre     = nPosNext +1;
	nPosNext    = str.Find(_T(","),nPosPre);
	Rect.right  = _tstoi(str.Mid(nPosPre,nPosNext-nPosPre));

	nPosPre     = nPosNext +1;
	nPosNext    = str.Find(_T(")"),nPosPre);
	Rect.bottom = _tstoi(str.Mid(nPosPre,nPosNext-nPosPre));

	*rtType = Rect;
}


void CUtilCommFuncFile::INI_GetInfoOfString(TCHAR *chBuf, double *dType)
{
	TCHAR *ptrEnd;
	*dType = (double)_tcstod(chBuf, &ptrEnd);
}


void CUtilCommFuncFile::INI_GetInfoOfString(TCHAR *chBuf, CRavidPoint<double> *dptType)
{
	int     nPosPre, nPosNext;
	CRavidPoint<double>  fPt;
	CString str;
	str.Format(_T("%s"),chBuf);

	nPosPre  = 1;
	nPosNext = str.Find(_T(","),nPosPre);
	fPt.x    = _tstof(str.Mid(nPosPre,nPosNext-nPosPre));

	nPosPre  = nPosNext +1;
	nPosNext = str.Find(_T(")"),nPosPre);
	fPt.y    = _tstof(str.Mid(nPosPre,nPosNext-nPosPre));

	*dptType = fPt;
}

void CUtilCommFuncFile::INI_GetInfoOfString(TCHAR *chBuf, float *fType)
{
	TCHAR *ptrEnd;
	*fType = (float)_tcstod(chBuf, &ptrEnd);

}

void CUtilCommFuncFile::INI_GetInfoOfString(TCHAR *chBuf, long *lType)
{
	TCHAR *ptrEnd;
	*lType = (long)_tcstod(chBuf, &ptrEnd);
}


void CUtilCommFuncFile::INI_GetInfoOfString(TCHAR *chBuf, int *nType)
{
	TCHAR *ptrEnd;
	*nType = (int)_tcstod(chBuf, &ptrEnd);
}


void CUtilCommFuncFile::INI_GetInfoOfString(TCHAR *chBuf, bool *bType)
{
	TCHAR *ptrEnd;
	*bType = (bool)(int)_tcstod(chBuf, &ptrEnd);
}

void CUtilCommFuncFile::INI_GetInfoOfString(TCHAR *chBuf, long long *nType)
{
	TCHAR *ptrEnd;
	*nType = (long long)_tcstod(chBuf, &ptrEnd);
}



void CUtilCommFuncFile::INI_SetInfoOfString(CString *strIni, CRavidPoint<int> ptType)
{
	strIni->Format(_T("(%d,%d)"), ptType.x, ptType.y);
}

void CUtilCommFuncFile::INI_SetInfoOfString(CString *strIni, RECT rtType)
{
	strIni->Format(_T("(%d,%d,%d,%d)"),rtType.left, rtType.top, rtType.right, rtType.bottom);
}


void CUtilCommFuncFile::INI_SetInfoOfString(CString *strIni, double dType)
{
	strIni->Format(_T("%.5f"), dType);	
}

void CUtilCommFuncFile::INI_SetInfoOfString(CString *strIni, CRavidPoint<double> dptType)
{
	strIni->Format(_T("(%.5f,%.5f)"), dptType.x, dptType.y);
}


void CUtilCommFuncFile::INI_SetInfoOfString(CString *strIni, float fType)
{
	strIni->Format(_T("%.5f"), fType);	
}


void CUtilCommFuncFile::INI_SetInfoOfString(CString *strIni, int nType)
{
	strIni->Format(_T("%d"), nType);	
}


void CUtilCommFuncFile::INI_SetInfoOfString(CString *strIni, bool bType)
{
	strIni->Format(_T("%d"), bType);	
}

void CUtilCommFuncFile::DoDelete(CString sPathName)
{
	CFileFind ff;
	CString sPath = sPathName;

	if(CheckPath(sPath) == PATH_IS_FILE)
	{
		if(!SetFileAttributes(sPathName, FILE_ATTRIBUTE_NORMAL))
			return;

		if(!DeleteFile(sPath)) throw new CFException(GetLastError());
		return;
	}

	PreparePath(sPath);
	sPath += _T("*.*");

	BOOL bRes = ff.FindFile(sPath);
	while(bRes)
	{
		bRes = ff.FindNextFile();
		if (ff.IsDots()) continue;
		if (ff.IsDirectory())
		{
			sPath = ff.GetFilePath();
			DoDelete(sPath);
		}
		else DoDelete(ff.GetFilePath());
	}
	ff.Close();
	if (!RemoveDirectory(sPathName) && !m_bAborted) throw new CFException(GetLastError());
}

void CUtilCommFuncFile::DoFolderCopy(CString sSourceFolder, CString sDestFolder, bool bDeleteAfterCopy)
{
	CFileFind ff;
	CString sPathSource = sSourceFolder;
	BOOL bRes = ff.FindFile(sPathSource);
	while (bRes)
	{
		bRes = ff.FindNextFile();
		if (ff.IsDots()) continue;
		if (ff.IsDirectory()) // source is a folder
		{
			if (m_iRecursionLimit == 0) continue;
			sPathSource = ff.GetFilePath() + CString(_T("\\")) + CString(_T("*.*"));
			CString sPathDest = sDestFolder + ff.GetFileName() + CString(_T("\\"));
			if (CheckPath(sPathDest) == PATH_NOT_FOUND) 
			{
				if (!CreateDirectory(sPathDest, NULL))
				{
					ff.Close();
					throw new CFException(GetLastError());
				}
			}
			if (m_iRecursionLimit > 0) m_iRecursionLimit --;
			DoFolderCopy(sPathSource, sPathDest, bDeleteAfterCopy);
		}
		else // source is a file
		{
			CString sNewFileName = sDestFolder + ff.GetFileName();
			DoFileCopy(ff.GetFilePath(), sNewFileName, bDeleteAfterCopy);
		}
	}
	ff.Close();
}

void CUtilCommFuncFile::DoCopy(CString sSource, CString sDest, bool bDeleteAfterCopy)
{
	CheckSelfRecursion(sSource, sDest);
	// source not found
	if (CheckPath(sSource) == PATH_NOT_FOUND)
	{
		CString sError = sSource + CString(_T(" not found"));
		throw new CFException(sError);
	}
	// dest not found
	if (CheckPath(sDest) == PATH_NOT_FOUND)
	{
		CString sError = sDest + CString(_T(" not found"));
		throw new CFException(sError);
	}
	// folder to file
	if (CheckPath(sSource) == PATH_IS_FOLDER && CheckPath(sDest) == PATH_IS_FILE) 
	{
		throw new CFException(_T("Wrong operation"));
	}
	// folder to folder
	if (CheckPath(sSource) == PATH_IS_FOLDER && CheckPath(sDest) == PATH_IS_FOLDER) 
	{
		CFileFind ff;
		CString sError = sSource + CString(_T(" not found"));
		PreparePath(sSource);
		PreparePath(sDest);
		sSource += _T("*.*");
		if (!ff.FindFile(sSource)) 
		{
			ff.Close();
			throw new CFException(sError);
		}
		if (!ff.FindNextFile()) 
		{
			ff.Close();
			throw new CFException(sError);
		}
		CString sFolderName = ParseFolderName(sSource);
		ff.Close();
		DoFolderCopy(sSource, sDest, bDeleteAfterCopy);
	}
	// file to file
	if (CheckPath(sSource) == PATH_IS_FILE && CheckPath(sDest) == PATH_IS_FILE) 
	{
		DoFileCopy(sSource, sDest);
	}
	// file to folder
	if (CheckPath(sSource) == PATH_IS_FILE && CheckPath(sDest) == PATH_IS_FOLDER) 
	{
		PreparePath(sDest);
		TCHAR drive[MAX_PATH], dir[MAX_PATH], name[MAX_PATH], ext[MAX_PATH];
		_tsplitpath_s(sSource, drive, dir, name, ext);
		sDest = sDest + CString(name) + CString(ext);
		DoFileCopy(sSource, sDest);
	}
}

int CUtilCommFuncFile::CheckPath(CString sPath)
{
	DWORD dwAttr = GetFileAttributes(sPath);
	if (dwAttr == 0xffffffff) 
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND) 
			return PATH_NOT_FOUND;

		return PATH_ERROR;
	}

	if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) 
		return PATH_IS_FOLDER;

	return PATH_IS_FILE;
}


void CUtilCommFuncFile::PreparePath(CString &sPath)
{
	if(sPath.Right(1) != _T("\\")) sPath += _T("\\");
}


bool CUtilCommFuncFile::CanDelete(CString sPathName)
{
	DWORD dwAttr = GetFileAttributes(sPathName);
	if (dwAttr == -1) return false;
	if (dwAttr & FILE_ATTRIBUTE_READONLY)
	{
		if (m_bAskIfReadOnly)
		{
			CString sTmp = sPathName;
			int pos = sTmp.ReverseFind(_T('\\'));
			if (pos != -1) sTmp.Delete(0, pos + 1);
			CString sText = sTmp + CString(_T(" is read olny. Do you want delete it?"));
			int iRes = MessageBox(NULL, sText, _T("Warning"), MB_YESNOCANCEL | MB_ICONQUESTION);
			switch (iRes)
			{
			case IDYES:
				{
					if (!SetFileAttributes(sPathName, FILE_ATTRIBUTE_NORMAL)) return false;
					return true;
				}
			case IDNO:
				{
					return false;
				}
			case IDCANCEL:
				{
					m_bAborted = true;
					throw new CFException(0);
					return false;
				}
			}
		}
		else
		{
			if (!SetFileAttributes(sPathName, FILE_ATTRIBUTE_NORMAL)) return false;
			return true;
		}
	}
	return true;
}

void CUtilCommFuncFile::DoFileCopy(CString sSourceFile, CString sDestFile, bool bDeleteAfterCopy)
{
	BOOL bOvrwriteFails = FALSE;

	if (!m_bOverwriteMode)
	{
		while (IsFileExist(sDestFile)) 
		{
			sDestFile = ChangeFileName(sDestFile);
		}
		bOvrwriteFails = TRUE;
	}

	if( !CopyFile(sSourceFile, sDestFile, bOvrwriteFails) )
		throw new CFException(GetLastError());
	else
		if(!SetFileAttributes(sDestFile, FILE_ATTRIBUTE_NORMAL))
			return;

	if( bDeleteAfterCopy )
		DoDelete(sSourceFile);
}

void CUtilCommFuncFile::CheckSelfRecursion(CString sSource, CString sDest)
{
	if (sDest.Find(sSource) != -1)
	{
		int i = 0, count1 = 0, count2 = 0;
		for(i = 0; i < sSource.GetLength(); i ++)	if (sSource[i] == '\\') count1 ++;
		for(i = 0; i < sDest.GetLength(); i ++)	if (sDest[i] == '\\') count2 ++;
		if (count2 >= count1) m_iRecursionLimit = count2 - count1;
	}
}

CString CUtilCommFuncFile::ParseFolderName(CString sPathName)
{
	CString sFolderName = sPathName;
	int pos = sFolderName.ReverseFind('\\');
	if (pos != -1) sFolderName.Delete(pos, sFolderName.GetLength() - pos);
	pos = sFolderName.ReverseFind('\\');
	if (pos != -1) sFolderName = sFolderName.Right(sFolderName.GetLength() - pos - 1);
	else sFolderName.Empty();
	return sFolderName;
}

CString CUtilCommFuncFile::ChangeFileName(CString sFileName)
{
	CString sName, sNewName, sResult;
	TCHAR drive[MAX_PATH];
	TCHAR dir  [MAX_PATH];
	TCHAR name [MAX_PATH];
	TCHAR ext  [MAX_PATH];
	_tsplitpath_s((LPCTSTR)sFileName, drive, dir, name, ext);
	sName = name;

	int pos = sName.Find(_T("Copy "));
	if (pos == -1)
	{
		sNewName = CString(_T("Copy of ")) + sName + CString(ext);
	}
	else
	{
		int pos1 = sName.Find('(');
		if (pos1 == -1)
		{
			sNewName = sName;
			sNewName.Delete(0, 8);
			sNewName = CString(_T("Copy (1) of ")) + sNewName + CString(ext);
		}
		else
		{
			CString sCount;
			int pos2 = sName.Find(')');
			if (pos2 == -1)
			{
				sNewName = CString(_T("Copy of ")) + sNewName + CString(ext);
			}
			else
			{
				sCount = sName.Mid(pos1 + 1, pos2 - pos1 - 1);
				sName.Delete(0, pos2 + 5);
				int iCount = _ttoi(sCount);
				iCount ++;
				sNewName.Format(_T("%s%d%s%s%s"), _T("Copy ("), iCount, _T(") of "), sName, ext);
			}
		}
	}

	sResult = CString(drive) + CString(dir) + sNewName;

	return sResult;
}



bool CUtilCommFuncFile::ReadTextFile(CString& filename, CString& contents)
{
	contents = _T( "" );

	CStdioFile file;
	CFileException feError;
	bool result = true;

	if( filename.IsEmpty() )
		return false;

	if( result )
	{
		// Reading the file
		if( file.Open( filename, CFile::modeRead, &feError ) )
		{

			CString line;
			while( file.ReadString( line ) )
				contents += line + '\n';

			file.Close();
		}
		else
		{
			result = false;
		}
	}

	return result;

}

bool CUtilCommFuncFile::WriteTextFile(CString& filename, const CString& contents)
{	
	CFile file;
	CFileException feError;

	bool result = true;

	if( filename.IsEmpty() )
		return false;

	if( result )
	{
		if( file.Open(filename, CFile::modeWrite | CFile::modeCreate, &feError) )
		{	
			// Write the file
			file.Write( contents, contents.GetLength() );
			file.Close();
		}
		else
		{
			result = false;
		}
	}

	return result;

}

bool CUtilCommFuncFile::AppendFile(CString& filename, const CString& contents)
{
	CFile file;
	CFileException feError;
	bool result = true;

	if( filename.IsEmpty() )
		result = false;

	if( result )
	{
		// Write the file
		if( file.Open( filename, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate, &feError ) )
		{
			file.SeekToEnd();
			file.Write( contents, contents.GetLength() );
			file.Close();

			int k = contents.GetLength();
			int jj=0;
		}
		else
		{
			result = false;
		}
	}

	return result;

}


bool CUtilCommFuncFile::MakeDirectory(CString strPathName)
{
	bool bReturn = true;

	int nStart = 0;

	while(true)
	{
		nStart = strPathName.Find(_T("\\"), nStart + 1);		//<-----  이건 왜 이렇게 되어 있을까??

		if(nStart < 0)
			break;

		CString strToCreateDir = strPathName.Left(nStart);

		CreateDirectory(strToCreateDir, nullptr);
		
		if(!IsFolderExist(strToCreateDir))
		{
			bReturn = false;
			break;
		}
	}

	return bReturn;
}


bool CUtilCommFuncFile::MakeDirectoryFullPath(CString strPathName)
{
	return CreateDirectory(strPathName, nullptr);
}


