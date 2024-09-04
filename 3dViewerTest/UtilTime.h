#pragma once
class CUtilTime
{
public:
	CUtilTime();
	~CUtilTime();

public:

	CString		GetStringOfCurrentMonth(LPCTSTR lpszDelimiter = nullptr, bool bIncludeUnit = false);
	CString		GetStringOfCurrentDay(LPCTSTR lpszDelimiter = nullptr, bool bIncludeUnit = false);

	CString		GetStringOfCurrentDayWithHour(LPCTSTR lpszDayDelimiter = nullptr, LPCTSTR lpszSeparator = nullptr);

	CString		GetStringOfCurrentTime(LPCTSTR lpszDelimiter = nullptr, bool bIncludeUnit = false);

	CString		GetStringOfCurrentDayTime(LPCTSTR lpszDayDelimiter = nullptr, LPCTSTR lpszTimeDelimiter = nullptr);

	CString		GetStringOfCurrentTimeWithMillisecond(LPCTSTR lpszDayDelimiter = nullptr, LPCTSTR lpszTerm = nullptr, LPCTSTR lpszTimeDelimiter = nullptr);

	CString		GetStringOfCurrentTimeWithDate();

	SYSTEMTIME	ConvertStringToSystemTime(CString strTime);

	long		ConvertCurrentTimeToTimeStamp();
	CString		ConvertTimeStampToCurrentTimeString(long nTimeStamp);

	ULONGLONG	GetTimeDifferenceWithSystemTime(SYSTEMTIME tSrc);

	bool		SetOSTime(CString strTime);

};

