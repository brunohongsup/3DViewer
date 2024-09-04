// UtilStringManager.cpp : CUtilStringManager 클래스를 구현한다.
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UtilStringManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////////////////
BOOL IsSigleParameter(CString strGet,int nPos,BOOL isArray)
{
	if(isArray)
		return FALSE;

	int  nFind;
	strGet = strGet.Left(nPos);
	nFind = strGet.Find(_T('"'), 0);
	if(nFind == -1)
		return TRUE;
	else
		return FALSE;

}
CUtilStringManager::CUtilStringManager(CString strSource, TCHAR chSeparator)
{	
	SetSplit(strSource, chSeparator);		
}

CUtilStringManager::CUtilStringManager(CString strSource, LPCTSTR lpszSeparator)
{
	SetSplit(strSource, lpszSeparator);
}
CUtilStringManager::CUtilStringManager(CString strSource, TCHAR chSeparator, TCHAR chSecSeparator)
{
	SetSplitQuestionMark(strSource, chSeparator, chSecSeparator);
}
CUtilStringManager::~CUtilStringManager()
{
	m_aryString.RemoveAll();
	m_aryElementCnt.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////
// 문자열을 분할후 분할된 수를 리턴한다.

INT_PTR CUtilStringManager::SetSplit(CString strSource, TCHAR chSeparator)
{
	// 기존에 분할되어있는 문자열들을 제거
	m_aryString.RemoveAll();
	m_aryElementCnt.RemoveAll();

	int nElementCnt =0;
	int nPos, nLenSep = 1;

	if (!strSource.IsEmpty())
	{
		while ((nPos = strSource.Find(chSeparator)) != -1)
		{
			nElementCnt++;
			m_aryString.Add(strSource.Left(nPos));
			strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
			m_aryElementCnt.Add(nElementCnt);
			nElementCnt = 0;
		}
		nElementCnt++;
		m_aryElementCnt.Add(1);
		if (strSource.IsEmpty()) m_aryString.Add(_T(""));
		else                     m_aryString.Add(strSource);
	}

	return m_aryString.GetSize();
}

INT_PTR CUtilStringManager::SetSplit(CString strSource, TCHAR chSeparator, TCHAR chSeparator2)
{
	// 기존에 분할되어있는 문자열들을 제거
	m_aryString.RemoveAll();
	m_aryElementCnt.RemoveAll();

	int nElementCnt = 0;
	int nPos, nLenSep = 1;
	int nEndPos = 0;

	if(!strSource.IsEmpty())
	{
		while((nPos = strSource.Find(chSeparator)) != -1)
		{
			nEndPos = strSource.Find(chSeparator2);
			if(nEndPos < 0)
				break;
			if(nEndPos > nPos)
			{
				nElementCnt++;
				int nWidth = nEndPos - nPos + 1;
				CString strAdd = strSource.Mid(nPos, nWidth);
				m_aryString.Add(strAdd);
				strSource = strSource.Right(strSource.GetLength() - nWidth - nPos);
				m_aryElementCnt.Add(nElementCnt);
				nElementCnt = 0;
			}		
			else
				break;
		}
	}

	return m_aryString.GetSize();
}
INT_PTR CUtilStringManager::SetSplit(CString strSource, LPCTSTR lpszSeparator)
{
	m_aryString.RemoveAll();
	m_aryElementCnt.RemoveAll();

	m_strSeparator = lpszSeparator;
	int nElementCnt =0;
	int nPos, nLenSep = (int)_tcslen(lpszSeparator);

	if (!strSource.IsEmpty())
	{
		while ((nPos = strSource.Find(lpszSeparator)) != -1)
		{
			nElementCnt++;
			m_aryString.Add(strSource.Left(nPos));
			strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
			m_aryElementCnt.Add(nElementCnt);
			nElementCnt = 0;
		}
		nElementCnt++;
		m_aryElementCnt.Add(1);
		if (strSource.IsEmpty()) m_aryString.Add(_T(""));
		else                     m_aryString.Add(strSource);
	}

	return m_aryString.GetSize();
}

INT_PTR CUtilStringManager::SetSplitQuestionMark(CString strSource, TCHAR chSeparator, TCHAR chSecSeparator)
{
	// 기존에 분할되어있는 문자열들을 제거
	CString strTemp;
	BOOL	bArrayElement = FALSE;
	int nPos, nLenSep = 1;
	int nQuestionPos;
	int nElementCnt =0;
	m_aryString.RemoveAll();
	m_aryElementCnt.RemoveAll();

	if (!strSource.IsEmpty())
	{
		while ((nPos = strSource.Find(chSeparator)) != -1)
		{
			if(nPos ==0 || strSource.GetAt(nPos-1) == _T(',') || IsSigleParameter(strSource,nPos,bArrayElement))
			{
				nElementCnt++;
				m_aryString.Add(strSource.Left(nPos));
				strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
				m_aryElementCnt.Add(nElementCnt);
				nElementCnt = 0;
				bArrayElement = FALSE;
			}
			else if(strSource.GetAt(nPos-1) == chSecSeparator)
			{
				nElementCnt++;
				strTemp += strSource.Left(nPos-1);
				if((nQuestionPos = strTemp.Find(chSecSeparator)) != -1)
					strTemp.Delete(nQuestionPos); //따옴표 제거

				m_aryString.Add(strTemp); //뒤쪽 따옴표 포함 안함
				strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));

				m_aryElementCnt.Add(nElementCnt);
				nElementCnt = 0;
				bArrayElement = FALSE;
				strTemp.Empty();
			}
			else
			{
				bArrayElement = TRUE;
				nElementCnt++;
				strTemp += strSource.Left(nPos+1);
				strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
				continue;
			}
		}

		if (strSource.IsEmpty()) m_aryString.Add(_T(""));
		else
		{
			strSource.Replace(_T("\n"),_T(","));
			nPos = strSource.Find(chSeparator);
			if(nPos != -1)
			{
				for(;;)
				{
					if(strSource.IsEmpty())
						break;
					if(nPos ==0 || strSource.GetAt(nPos-1) == _T(',') || IsSigleParameter(strSource,nPos,bArrayElement))
					{
						nElementCnt++;
						m_aryString.Add(strSource.Left(nPos));
						strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
						m_aryElementCnt.Add(nElementCnt);
						nElementCnt = 0;
						bArrayElement = FALSE;
					}
					else if(strSource.GetAt(nPos-1) == chSecSeparator)
					{
						nElementCnt++;
						strTemp += strSource.Left(nPos-1);
						if((nQuestionPos = strTemp.Find(chSecSeparator)) != -1)
							strTemp.Delete(nQuestionPos); //따옴표 제거

						m_aryString.Add(strTemp); //뒤쪽 따옴표 포함 안함
						strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));

						m_aryElementCnt.Add(nElementCnt);
						nElementCnt = 0;
						bArrayElement = FALSE;
						strTemp.Empty();
						break;
					}
					else
					{
						bArrayElement = TRUE;
						nElementCnt++;
						strTemp += strSource.Left(nPos+1);
						strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
						continue;
					}
				}
			}
		}
	}

	return m_aryString.GetSize();
}
////////////////////////////////////////////////////////////////////////////////
// 문자열에서 지정한 문자열 앞에 원하는 문자열을 삽입한다.

CString CUtilStringManager::InsertString(CString strSource, LPCTSTR lpszBefore, LPCTSTR lpszInsert)
{
	int x;
	CString strTemp1, strTemp2;

	if ((x = strSource.Find(lpszBefore)) != -1)
	{
		strTemp1 = strSource.Mid(0, x);
		strTemp2 = strSource.Mid(x + 1, strSource.GetLength() - x);
		strSource = strTemp1 + lpszInsert + strTemp2;
	}

	return strSource;
}

////////////////////////////////////////////////////////////////////////////////
// 문자열에서 일치되는 문자열들을 제거한다.

CString CUtilStringManager::DeleteString(CString strSource, CString strDelete)
{
	int x, nLenSur;
	int nLenDel = strDelete.GetLength();

	CString strTemp1, strTemp2;

	while ((x = strSource.Find(strDelete)) != -1)
	{
		nLenSur = strSource.GetLength();

		strTemp1 = strSource.Mid(0, x);
		strTemp2 = strSource.Mid(x + nLenDel, nLenSur - x + nLenDel);

		strSource = strTemp1 + strTemp2;
	}

	return strSource;
}

size_t CUtilStringManager::CharCount(LPCTSTR lpszString, TCHAR ch)
{
	size_t i, nCount = 0;

	for (i = 0; i < _tcslen(lpszString); i++)
	{
		if (*(lpszString + i) == ch) nCount++;
	}

	return nCount;
}

////////////////////////////////////////////////////////////////////////////////

CString CUtilStringManager::FloatString(double fValue)
{
	CString strText;

	strText.Format(_T("%f"), fValue);
	strText.TrimRight(_T('0'));
	strText.TrimRight(_T('.'));

	return strText;
}

////////////////////////////////////////////////////////////////////////////////
//kyubun start
BOOL CUtilStringManager::SetSplitString(CString strInsert, int nIndex)
{
	if(nIndex > GetCount()) return FALSE;
	m_aryString.SetAt(nIndex, strInsert);

	return TRUE;
}

CString CUtilStringManager::GetSourceString()
{
	int i;
	m_strSourceString = _T("");

	for(i = 0; i < GetCount(); i++)
	{
		if( m_strSourceString.IsEmpty() )
			m_strSourceString += m_aryString.GetAt(i);
		else
			m_strSourceString += (m_strSeparator + m_aryString.GetAt(i));
	}
	return m_strSourceString;
}

