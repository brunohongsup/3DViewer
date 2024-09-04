#include "stdafx.h"
#include "Util.h"


CUtil::CUtil()
{
	m_aryString.RemoveAll();
}


CUtil::~CUtil()
{
}

CString CUtil::ConvertRectStructureToString(CRavidGeometry * pGeo)
{
	CString strResult; 
	strResult.Empty();

	do 
	{
		CRavidRect<double>* pRect = dynamic_cast<CRavidRect<double>*>(pGeo);
		if(!pRect)
			break;

		CString strSeparator;
		strSeparator = _T("/");

		strResult.Format(_T("%.1f%s%.1f%s%.1f%s%.1f"), pRect->left, strSeparator, pRect->top, strSeparator, pRect->right, strSeparator, pRect->bottom);


	} while (false);
	
	return strResult;
}

bool CUtil::ConvertRectStringToRectStructure(CString strData, CString strSeprator, CRavidRect<double>* pResult)
{
	do 
	{
		if(strData.IsEmpty())
			break;

		if(strSeprator.IsEmpty())
			break;

		if(!pResult)
			break;

		CString strSrc;
		strSrc.Format(_T("%s"), strData);


		CStringArray arrString;
		arrString.RemoveAll();

		int nCount = 0;
		int nPos = 1;
		int nLenSep = (int)_tcslen(strSeprator);

		while((nPos = strSrc.Find(strSeprator)) != -1)
		{
			++nCount;
			arrString.Add(strSrc.Left(nPos));
			strSrc = strSrc.Right(strSrc.GetLength() - (nPos + nLenSep));
		}

		if(strSrc.IsEmpty())
			arrString.Add(_T(""));
		else
			arrString.Add(strSrc);
		
		++nCount;

		if(nCount != 4)
			break;

		pResult->left = _ttof(arrString.ElementAt(0));
		pResult->top = _ttof(arrString.ElementAt(1));
		pResult->right = _ttof(arrString.ElementAt(2));
		pResult->bottom = _ttof(arrString.ElementAt(3));
		

	} while (false);
	
	return pResult;
}

bool CUtil::ConvertRectStringToRectStructure(CString strData, CString strSeprator, CRavidRect<int>* pResult)
{
	do
	{
		if (strData.IsEmpty())
			break;

		if (strSeprator.IsEmpty())
			break;

		if (!pResult)
			break;

		CString strSrc;
		strSrc.Format(_T("%s"), strData);


		CStringArray arrString;
		arrString.RemoveAll();

		int nCount = 0;
		int nPos = 1;
		int nLenSep = (int)_tcslen(strSeprator);

		while ((nPos = strSrc.Find(strSeprator)) != -1)
		{
			++nCount;
			arrString.Add(strSrc.Left(nPos));
			strSrc = strSrc.Right(strSrc.GetLength() - (nPos + nLenSep));
		}

		if (strSrc.IsEmpty())
			arrString.Add(_T(""));
		else
			arrString.Add(strSrc);

		++nCount;

		if (nCount != 4)
			break;

		pResult->left = _ttoi(arrString.ElementAt(0));
		pResult->top = _ttoi(arrString.ElementAt(1));
		pResult->right = _ttoi(arrString.ElementAt(2));
		pResult->bottom = _ttoi(arrString.ElementAt(3));


	} while (false);

	return pResult;
}

bool CUtil::ConvertRectStringToPointStructure(CString strData, CString strSeprator, CRavidPoint<double>* pResult)
{
	do
	{
		if(strData.IsEmpty())
			break;

		if(strSeprator.IsEmpty())
			break;

		if(!pResult)
			break;

		CString strSrc;
		strSrc.Format(_T("%s"), strData);


		CStringArray arrString;
		arrString.RemoveAll();

		int nCount = 0;
		int nPos = 1;
		int nLenSep = (int)_tcslen(strSeprator);

		while((nPos = strSrc.Find(strSeprator)) != -1)
		{
			++nCount;
			arrString.Add(strSrc.Left(nPos));
			strSrc = strSrc.Right(strSrc.GetLength() - (nPos + nLenSep));
		}

		if(strSrc.IsEmpty())
			arrString.Add(_T(""));
		else
			arrString.Add(strSrc);

		++nCount;

		if(nCount != 2)
			break;

		pResult->x = _ttof(arrString.ElementAt(0));
		pResult->y = _ttof(arrString.ElementAt(1));
		
	}
	while(false);

	return pResult;
}

bool CUtil::ConvertIntStringToIntStructure(CString strData, CString strSeprator, int pResult[])
{
	do
	{
		if (strData.IsEmpty())
			break;

		if (strSeprator.IsEmpty())
			break;

		if (!pResult)
			break;

		CString strSrc;
		strSrc.Format(_T("%s"), strData);


		CStringArray arrString;
		arrString.RemoveAll();

		int nCount = 0;
		int nPos = 1;
		int nLenSep = (int)_tcslen(strSeprator);

		while ((nPos = strSrc.Find(strSeprator)) != -1)
		{
			++nCount;
			arrString.Add(strSrc.Left(nPos));
			strSrc = strSrc.Right(strSrc.GetLength() - (nPos + nLenSep));
		}

		if (strSrc.IsEmpty())
			arrString.Add(_T(""));
		else
			arrString.Add(strSrc);

		++nCount;

		for(int i = 0 ; i < nCount; i++)
		{
			pResult[i] = _ttoi(arrString.ElementAt(i));
		}

	} while (false);

	return pResult;
}

CString CUtil::GetStringOfCurrentTime()
{
	CString strResult;
	strResult.Empty();

	CTime time(CTime::GetCurrentTime());
	strResult.Format(_T("%s"), time.Format("%H:%M:%S"));


	return strResult;
}

bool CUtil::SetOSTime(CString strTime)
{

	bool bReturn = false;
	do
	{
		if(strTime.GetLength() < 14)
			break;

		SYSTEMTIME	sysTime;
		ZeroMemory(&sysTime, sizeof(sysTime));

		bool bSet = false;

		for(int i = 0; i < 6; ++i)
		{
			switch(i)
			{
			case 0:
				sysTime.wYear = _ttoi(strTime.Left(4));
				break;
			case 1:
				sysTime.wMonth = _ttoi(strTime.Mid(4, 2));
				break;
			case 2:
				sysTime.wDay = _ttoi(strTime.Mid(6, 2));
				break;
			case 3:
				sysTime.wHour = _ttoi(strTime.Mid(8, 2));
				break;;
			case 4:
				sysTime.wMinute = _ttoi(strTime.Mid(10, 2));
				break;
			case 5:
				sysTime.wSecond = _ttoi(strTime.Mid(12, 2));
				bSet = true;
				break;
			default:
				break;
			}
		}

		if(bSet)
		{
			bReturn = SetLocalTime(&sysTime);
		}

	}
	while(false);

	return bReturn;
}

int CUtil::SetSplit(CString strSource, CString strSeparator)
{
	m_aryString.RemoveAll();

	int nElementCnt = 0;
	int nPos, nLenSep = (int)_tcslen(strSeparator);

	if(!strSource.IsEmpty())
	{
		while((nPos = strSource.Find(strSeparator)) != -1)
		{
			nElementCnt++;
			m_aryString.Add(strSource.Left(nPos));
			strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
			nElementCnt = 0;
		}

		nElementCnt++;
		if(strSource.IsEmpty()) m_aryString.Add(_T(""));
		else                     m_aryString.Add(strSource);
	}

	return (int)m_aryString.GetSize();
}


int CUtil::GetCPUCount()
{
	SYSTEM_INFO systemInfo;
	CString strTemp;

	GetSystemInfo(&systemInfo);

	return systemInfo.dwNumberOfProcessors;
}


