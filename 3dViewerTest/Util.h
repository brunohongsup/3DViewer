#pragma once
class CUtil
{
public:
	CUtil();
	~CUtil();

public:
	CString		ConvertRectStructureToString(CRavidGeometry* pGeo);
	bool		ConvertRectStringToRectStructure(CString strData, CString strSeprator, CRavidRect<double>* pResult);
	bool		ConvertRectStringToRectStructure(CString strData, CString strSeprator, CRavidRect<int>* pResult);
	bool		ConvertRectStringToPointStructure(CString strData, CString strSeprator, CRavidPoint<double>* pResult);
	bool		ConvertIntStringToIntStructure(CString strData, CString strSeprator, int pResult[]);


	CString		GetStringOfCurrentTime();
	bool		SetOSTime(CString strTime);

	int			SetSplit(CString strSource, CString strSeparator);
	CString		GetSplit(int nIndex)		{	return m_aryString.GetAt(nIndex);	}

	int			GetCPUCount();

private:
	CStringArray	m_aryString;
};

