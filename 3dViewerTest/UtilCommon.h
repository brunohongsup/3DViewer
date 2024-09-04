#pragma once

using namespace Ravid;
using namespace Ravid::Framework;

class CUtilCommon
{
public:
	CUtilCommon();
	~CUtilCommon();

public:
	CString			ConvertRavidPointToString(CRavidGeometry* pGeo, CString strSeparator);
	CString			ConvertRavidLineToString(CRavidGeometry* pGeo, CString strSeparator, CString strDelimiter);
	CString			ConvertRavidRectToString(CRavidGeometry* pGeo, CString strSeparator);
	CString			ConvertRavidQuadangleToString(CRavidGeometry* pGeo, CString strSeparator, CString strDelimiter);
	CString			ConvertRavidCircleToString(CRavidGeometry* pGeo, CString strSeparator, CString strDelimiter);
	CString			ConvertRavidEllipseToString(CRavidGeometry* pGeo, CString strSeparator, CString strDelimiter);
	CString			ConvertRavidPolygonToString(CRavidGeometryArray* pGeoArray, CString strSeparator, CString strDelimiter, CString strShapeSeparator, CString strArraySeparator);


	bool			ConvertStringToRavidPoint(CString strParam, CString strSeprator, CRavidPoint<int>* pResult);
	bool			ConvertStringToRavidPoint(CString strParam, CString strSeprator, CRavidPoint<long long>* pResult);
	bool			ConvertStringToRavidPoint(CString strParam, CString strSeprator, CRavidPoint<float>* pResult);
	bool			ConvertStringToRavidPoint(CString strParam, CString strSeprator, CRavidPoint<double>* pResult);

	bool			ConvertStringToRavidLine(CString strParam, CString strSeprator, CString strDelimiter, CRavidLine<int>* pResult);
	bool			ConvertStringToRavidLine(CString strParam, CString strSeprator, CString strDelimiter, CRavidLine<long long>* pResult);
	bool			ConvertStringToRavidLine(CString strParam, CString strSeprator, CString strDelimiter, CRavidLine<float>* pResult);
	bool			ConvertStringToRavidLine(CString strParam, CString strSeprator, CString strDelimiter, CRavidLine<double>* pResult);

	bool			ConvertStringToRavidRect(CString strParam, CString strSeprator, CRavidRect<int>* pResult);
	bool			ConvertStringToRavidRect(CString strParam, CString strSeprator, CRavidRect<long long>* pResult);
	bool			ConvertStringToRavidRect(CString strParam, CString strSeprator, CRavidRect<float>* pResult);
	bool			ConvertStringToRavidRect(CString strParam, CString strSeprator, CRavidRect<double>* pResult);

	bool			ConvertStringToDouble(CString strParam, CString strSeparator, int nTotalCount, double* pResult);
	bool			ConvertStringToInt(CString strParam, CString strSeparator, int nTotalCount, int* pResult);
	bool			ConvertStringToString(CString strParam, CString strSeparator, int nTotalCount, CString* pResult);
	bool			ConvertStringToString(CString strParam, CString strSeparator, int nTotalCount, CString strInit, CString* pResult);
	bool			ConvertStringToVctPoints(CString strParam, CString strSeparator, std::vector<CRavidPoint<double>>* pResult);

	bool			ConvertStringToRavidQuadangle(CString strParam, CString strSeprator, CString strDelimiter, CRavidQuadrangle<int>* pResult);
	bool			ConvertStringToRavidQuadangle(CString strParam, CString strSeprator, CString strDelimiter, CRavidQuadrangle<long long>* pResult);
	bool			ConvertStringToRavidQuadangle(CString strParam, CString strSeprator, CString strDelimiter, CRavidQuadrangle<float>* pResult);
	bool			ConvertStringToRavidQuadangle(CString strParam, CString strSeprator, CString strDelimiter, CRavidQuadrangle<double>* pResult);

	bool			ConvertStringToRavidCircle(CString strParam, CString strSeprator, CString strDelimiter, CRavidCircle<int>* pResult);
	bool			ConvertStringToRavidCircle(CString strParam, CString strSeprator, CString strDelimiter, CRavidCircle<long long>* pResult);
	bool			ConvertStringToRavidCircle(CString strParam, CString strSeprator, CString strDelimiter, CRavidCircle<float>* pResult);
	bool			ConvertStringToRavidCircle(CString strParam, CString strSeprator, CString strDelimiter, CRavidCircle<double>* pResult);


	bool			ConvertStringToRavidEllipse(CString strParam, CString strSeprator, CString strDelimiter, CRavidEllipse<int>* pResult);
	bool			ConvertStringToRavidEllipse(CString strParam, CString strSeprator, CString strDelimiter, CRavidEllipse<long long>* pResult);
	bool			ConvertStringToRavidEllipse(CString strParam, CString strSeprator, CString strDelimiter, CRavidEllipse<float>* pResult);
	bool			ConvertStringToRavidEllipse(CString strParam, CString strSeprator, CString strDelimiter, CRavidEllipse<double>* pResult);

	bool			ConvertStringToRavidPolygon(CString strParam, CString strSeparator, CString strDelimiter, CString strShapeSeparator, CString strArraySeparator, CRavidGeometryArray* pResult);

	bool			ConvertRavidGeometryToString(CRavidGeometry* pGeo, CString& strShape);
	CRavidGeometry* ConvertStringToRavidGeometry(CString strParam);


	const CString&	GetRootPath();

protected:
	CRavidPoint<double> m_rpPoint;
	CRavidLine<double> m_rlLine;
	CRavidRect<double> m_rrRect;
	CRavidQuadrangle<double> m_rqQuadrangle;
	CRavidCircle<double> m_rcCircle;
	CRavidEllipse<double> m_reEllipse;
	CRavidPolygon m_rPolygon;

};

