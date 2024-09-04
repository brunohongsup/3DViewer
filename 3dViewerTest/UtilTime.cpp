#include "stdafx.h"
#include "UtilTime.h"

#include <time.h>

CUtilTime::CUtilTime()
{

}

CUtilTime::~CUtilTime()
{
}


CString CUtilTime::GetStringOfCurrentMonth(LPCTSTR lpszDelimiter/* = nullptr*/, bool bIncludeUnit/* = false*/)
{

	CString strResult;
	strResult.Empty();

	CTime time(CTime::GetCurrentTime());

	CString strDelimiter;
	if(lpszDelimiter)
		strDelimiter.Format(_T("%s"), lpszDelimiter);
	else
		strDelimiter = _T("");

	CStringArray astrUnit;
	if(bIncludeUnit)
	{
		astrUnit.Add(_T("Y"));
		astrUnit.Add(_T("M"));
	}
	else
	{
		astrUnit.Add(_T(""));
		astrUnit.Add(_T(""));
	}

	strResult.Format(_T("%04d%s%s%02d%s"),
					 time.GetYear(), astrUnit.GetAt(0), strDelimiter, time.GetMonth(), astrUnit.GetAt(1));

	return strResult;
}


CString CUtilTime::GetStringOfCurrentDay(LPCTSTR lpszDelimiter/* = nullptr*/, bool bIncludeUnit/* = false*/)
{

	CString strResult;
	strResult.Empty();

	CTime time(CTime::GetCurrentTime());

	CString strDelimiter;
	if(lpszDelimiter)
		strDelimiter.Format(_T("%s"), lpszDelimiter);
	else
		strDelimiter = _T("");

	CStringArray astrUnit;
	if(bIncludeUnit)
	{
		astrUnit.Add(_T("Y"));
		astrUnit.Add(_T("M"));
		astrUnit.Add(_T("D"));
	}
	else
	{
		astrUnit.Add(_T(""));
		astrUnit.Add(_T(""));
		astrUnit.Add(_T(""));
	}

	strResult.Format(_T("%04d%s%s%02d%s%s%02d%s"),
					 time.GetYear(), astrUnit.GetAt(0), strDelimiter, time.GetMonth(), astrUnit.GetAt(1), strDelimiter, time.GetDay(), astrUnit.GetAt(2));

	return strResult;
}



CString CUtilTime::GetStringOfCurrentDayWithHour(LPCTSTR lpszDayDelimiter, LPCTSTR lpszSeparator)
{

	CString strResult;
	strResult.Empty();

	CTime time(CTime::GetCurrentTime());

	CString strDayDelimiter;
	if(lpszDayDelimiter)
		strDayDelimiter.Format(_T("%s"), lpszDayDelimiter);
	else
		strDayDelimiter = _T("");

	CString strSeparator;
	if(lpszSeparator)
		strSeparator.Format(_T("%s"), lpszSeparator);
	else
		strSeparator = _T("");


	strResult.Format(_T("%04d%s%02d%s%02d%s%02d"),
					 time.GetYear(), strDayDelimiter, time.GetMonth(), strDayDelimiter, time.GetDay(), strSeparator, time.GetHour());

	return strResult;
}




CString CUtilTime::GetStringOfCurrentTime(LPCTSTR lpszDelimiter/* = nullptr*/, bool bIncludeUnit/* = false*/)
{
	CString strResult;
	strResult.Empty();

	CTime time(CTime::GetCurrentTime());

	CString strDelimiter;
	if(lpszDelimiter)
		strDelimiter.Format(_T("%s"), lpszDelimiter);
	else
		strDelimiter = _T("");

	CStringArray astrUnit;
	if(bIncludeUnit)
	{
		astrUnit.Add(_T("h"));
		astrUnit.Add(_T("m"));
		astrUnit.Add(_T("s"));
	}
	else
	{
		astrUnit.Add(_T(""));
		astrUnit.Add(_T(""));
		astrUnit.Add(_T(""));
	}



	strResult.Format(_T("%02d%s%s%02d%s%s%02d%s"),
					 time.GetHour(), astrUnit.GetAt(0), strDelimiter, time.GetMinute(), astrUnit.GetAt(1), strDelimiter, time.GetSecond(), astrUnit.GetAt(2));

	return strResult;
}

CString CUtilTime::GetStringOfCurrentDayTime(LPCTSTR lpszDayDelimiter/* = nullptr*/, LPCTSTR lpszTimeDelimiter/* = nullptr*/)
{
	CString strResult;
	strResult.Empty();

	CTime time = CTime::GetCurrentTime();

	CString strDayDelimiter;
	CString strTimeDelimiter;

	if(lpszDayDelimiter)
		strDayDelimiter.Format(_T("%s"), lpszDayDelimiter);
	else
		strDayDelimiter = _T("");

	if(lpszTimeDelimiter)
		strTimeDelimiter.Format(_T("%s"), lpszTimeDelimiter);
	else
		strTimeDelimiter = _T("");


	strResult.Format(_T("%04d%s%02d%s%02d %02d%s%02d%s%02d"),
					 time.GetYear(), strDayDelimiter, time.GetMonth(), strDayDelimiter, time.GetDay(),
					 time.GetHour(), strTimeDelimiter, time.GetMinute(), strTimeDelimiter, time.GetSecond());

	return strResult;
}

CString CUtilTime::GetStringOfCurrentTimeWithMillisecond(LPCTSTR lpszDayDelimiter, LPCTSTR lpszTerm, LPCTSTR lpszTimeDelimiter)
{
	// GEM Clock : MES 전용..,

	CString strResult;
	strResult.Empty();

	SYSTEMTIME st;
	GetLocalTime(&st);

	CString strDayDelimiter;
	CString strTerm;
	CString strTimeDelimiter;

	if(lpszDayDelimiter)
		strDayDelimiter.Format(_T("%s"), lpszDayDelimiter);
	else
		strDayDelimiter = _T("");

	if(lpszTerm)
		strTerm.Format(_T("%s"), lpszTerm);
	else
		strTerm = _T("");

	if(lpszTimeDelimiter)
		strTimeDelimiter.Format(_T("%s"), lpszTimeDelimiter);
	else
		strTimeDelimiter = _T("");

	CString strValue;
	strValue.Format(_T("%04d"), st.wYear);
	strResult += strValue;
	strResult += strDayDelimiter;
	strValue.Format(_T("%02d"), st.wMonth);
	strResult += strValue;
	strResult += strDayDelimiter;
	strValue.Format(_T("%02d"), st.wDay);
	strResult += strValue;

	strResult += strTerm;

	strValue.Format(_T("%02d"), st.wHour);
	strResult += strValue;
	strResult += strTimeDelimiter;
	strValue.Format(_T("%02d"), st.wMinute);
	strResult += strValue;
	strResult += strTimeDelimiter;
	strValue.Format(_T("%02d"), st.wSecond);
	strResult += strValue;
	strResult += strTimeDelimiter;
	strValue.Format(_T("%03d"), st.wMilliseconds);
	strResult += strValue;


	return strResult;
}

CString CUtilTime::GetStringOfCurrentTimeWithDate()
{
	// GEM Clock : MES 전용..,

	CString strResult;
	strResult.Empty();

	SYSTEMTIME st;
	//GetSystemTime(&st);
	GetLocalTime(&st);

	CString strValue;
	strValue.Format(_T("%04d"), st.wYear);
	strResult += strValue;
	strValue.Format(_T("%02d"), st.wMonth);
	strResult += strValue;
	strValue.Format(_T("%02d"), st.wDay);
	strResult += strValue;
	strValue.Format(_T("%02d"), st.wHour);
	strResult += strValue;
	strValue.Format(_T("%02d"), st.wMinute);
	strResult += strValue;
	strValue.Format(_T("%02d"), st.wSecond);
	strResult += strValue;

	return strResult;
}


SYSTEMTIME CUtilTime::ConvertStringToSystemTime(CString strTime)
{
	SYSTEMTIME sReturn;
	ZeroMemory(&sReturn, sizeof(sReturn));

	do
	{
		int nLength = strTime.GetLength();

		if(nLength != 14 || nLength != 16)
			break;


		int nCount = (nLength == 14) ? 6 : 7;

		for(int i = 0; i < nCount; ++i)
		{
			switch(i)
			{
			case 0:
				sReturn.wYear = _ttoi(strTime.Left(4));
				break;
			case 1:
				sReturn.wMonth = _ttoi(strTime.Mid(4, 2));
				break;
			case 2:
				sReturn.wDay = _ttoi(strTime.Mid(6, 2));
				break;
			case 3:
				sReturn.wHour = _ttoi(strTime.Mid(8, 2));
				break;;
			case 4:
				sReturn.wMinute = _ttoi(strTime.Mid(10, 2));
				break;
			case 5:
				sReturn.wSecond = _ttoi(strTime.Mid(12, 2));
				break;
			case 6:
				sReturn.wMilliseconds = _ttoi(strTime.Mid(14, 2));
				break;
			default:
				break;
			}
		}

	}
	while(false);

	return sReturn;
}


long CUtilTime::ConvertCurrentTimeToTimeStamp()
{
	long nReturn = 0;

	CTime tCurrent = CTime::GetCurrentTime();

	int nYear = tCurrent.GetYear();
	int nMonth = tCurrent.GetMonth();
	int nDay = tCurrent.GetDay();
	int nHour = tCurrent.GetHour();
	int nMinute = tCurrent.GetMinute();
	int nSecond = tCurrent.GetSecond();

	struct tm t1 = { 0, };
	t1.tm_year = nYear - 1900;
	t1.tm_mon = nMonth - 1;
	t1.tm_mday = nDay;
	t1.tm_hour = nHour;
	t1.tm_min = nMinute;
	t1.tm_sec = nSecond;

	nReturn = (long)mktime(&t1);

	return nReturn;
}

CString CUtilTime::ConvertTimeStampToCurrentTimeString(long nTimeStamp)
{
	//time_t now = time(NULL);

	CString strReturn;
	strReturn.Empty();

	time_t t = nTimeStamp;
	struct tm tData;

	localtime_s(&tData, &t);

	CString strValue;
	strValue.Format(_T("%04d"), tData.tm_year + 1900);
	strReturn += strValue;

	strValue.Format(_T("%02d"), tData.tm_mon + 1);
	strReturn += strValue;

	strValue.Format(_T("%02d"), tData.tm_mday);
	strReturn += strValue;

	strValue.Format(_T("%02d"), tData.tm_hour);
	strReturn += strValue;

	strValue.Format(_T("%02d"), tData.tm_min);
	strReturn += strValue;

	strValue.Format(_T("%02d"), tData.tm_sec);
	strReturn += strValue;



	return strReturn;
}

ULONGLONG CUtilTime::GetTimeDifferenceWithSystemTime(SYSTEMTIME tSrc)
{
	ULONGLONG	ullReturn = 0;

	FILETIME fileTime;
	ULARGE_INTEGER uliSrc, uliCurrent;
	ULONGLONG ullSrc, ullCurrent;


	// Source Time...
	SystemTimeToFileTime(&tSrc, &fileTime);
	uliSrc.LowPart = fileTime.dwLowDateTime;
	uliSrc.HighPart = fileTime.dwHighDateTime;
	ullSrc = uliSrc.QuadPart;


	/// Current Time..
	SYSTEMTIME tCurrent;
	ZeroMemory(&tCurrent, sizeof(tCurrent));

	CTime cTimeCurrent = CTime::GetCurrentTime();
	tCurrent.wYear = cTimeCurrent.GetYear();
	tCurrent.wMonth = cTimeCurrent.GetMonth();
	tCurrent.wDay = cTimeCurrent.GetDay();
	tCurrent.wHour = cTimeCurrent.GetHour();
	tCurrent.wMinute = cTimeCurrent.GetMinute();
	tCurrent.wSecond = cTimeCurrent.GetSecond();

	SystemTimeToFileTime(&tCurrent, &fileTime);
	uliCurrent.LowPart = fileTime.dwLowDateTime;
	uliCurrent.HighPart = fileTime.dwHighDateTime;
	ullCurrent = uliCurrent.QuadPart;

	ullReturn = (ullCurrent - ullSrc) / 10000;		// ms
	ullReturn /= 1000;								// s
	ullReturn /= 60;								// m
	ullReturn /= 60;								// h
	ullReturn /= 24;								// day

	return ullReturn;
}


bool CUtilTime::SetOSTime(CString strTime)
{

	bool bReturn = false;
	do
	{
		int nLength = strTime.GetLength();

		if(nLength != 14 || nLength != 16)
			break;

		SYSTEMTIME	sysTime;
		ZeroMemory(&sysTime, sizeof(sysTime));


		int nCount = (nLength == 14) ? 6 : 7;

		bool bSet = false;

		for(int i = 0; i < nCount; ++i)
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
				break;
			case 6:
				sysTime.wMilliseconds = _ttoi(strTime.Mid(14, 2));
				bSet = true;
				break;
			default:
				break;
			}
		}

		if(!bSet)
			break;

		bReturn = SetLocalTime(&sysTime);
	}
	while(false);

	return bReturn;
}