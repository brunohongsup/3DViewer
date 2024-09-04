// UtilStringManager.h : CUtilStringManager 클래스를 정의한다.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

class CUtilStringManager
{
private:
	CStringArray	m_aryString;			// 분할된 문자열을 관리
	CString			m_strSeparator;
	CString			m_strSourceString;
	CUIntArray		m_aryElementCnt;

public:
	CUtilStringManager() {}
	CUtilStringManager(CString strSource, TCHAR chSeparator);
	CUtilStringManager(CString strSource, LPCTSTR lpszSeparator);
	CUtilStringManager(CString strSource, TCHAR chSeparator, TCHAR chSecSeparator);
	virtual ~CUtilStringManager();

	INT_PTR		SetSplit(CString strSource, TCHAR chSeparator);
	INT_PTR		SetSplit(CString strSource, TCHAR chSeparator, TCHAR chSeparator2);
	INT_PTR		SetSplit(CString strSource, LPCTSTR lpszSeparator);
	INT_PTR		SetSplitQuestionMark(CString strSource, TCHAR chSeparator, TCHAR chSecSeparator);

	INT_PTR		GetCount() { return m_aryString.GetSize(); }
	CString		GetSplit(int nIndex) { return m_aryString.GetAt(nIndex); }
	int 		GetElementCount(int nIndex) { return m_aryElementCnt.GetAt(nIndex); }

	static size_t CharCount(LPCTSTR lpszString, TCHAR ch);
	static CString InsertString(CString strSource, LPCTSTR lpszBefore, LPCTSTR lpszInsert);
	static CString DeleteString(CString strSource, CString strDelete);
	static CString FloatString(double fValue);



//20041118 kyubun
	BOOL SetSplitString(CString strInsert, int nIndex);
	CString GetSourceString();
};
