#include "stdafx.h"
#include "UtilCommon.h"

#include "UtilStringManager.h"


CUtilCommon::CUtilCommon()
{
}


CUtilCommon::~CUtilCommon()
{
	m_rpPoint;
	m_rlLine;
	m_rrRect;
	m_rqQuadrangle;
	m_rcCircle;
	m_reEllipse;
	m_rPolygon;
}

CString CUtilCommon::ConvertRavidPointToString(CRavidGeometry * pGeo, CString strSeparator)
{
	CString strResult;
	strResult.Empty();
	do
	{
		ERavidGeometryTemplateType eType = pGeo->GetTemplateType();

		switch(eType)
		{
		case ERavidGeometryTemplateType_Unknown:
			break;
		case ERavidGeometryTemplateType_Int:
			{
				CRavidPoint<int>* pPoint = dynamic_cast<CRavidPoint<int>*>(pGeo);
				if(!pPoint)
					break;
				strResult.Format(_T("%d%s%d"), pPoint->x, strSeparator, pPoint->y);
			}
			break;
		case ERavidGeometryTemplateType_LongLong:
			{
				CRavidPoint<long long>* pPoint = dynamic_cast<CRavidPoint<long long>*>(pGeo);
				if(!pPoint)
					break;
				strResult.Format(_T("%lld%s%lld"), pPoint->x, strSeparator, pPoint->y);
			}
			break;
		case ERavidGeometryTemplateType_Float:
			{
				CRavidPoint<float>* pPoint = dynamic_cast<CRavidPoint<float>*>(pGeo);
				if(!pPoint)
					break;
				strResult.Format(_T("%.1f%s%.1f"), pPoint->x, strSeparator, pPoint->y);
			}
			break;
		case ERavidGeometryTemplateType_Double:
			{
				CRavidPoint<double>* pPoint = dynamic_cast<CRavidPoint<double>*>(pGeo);
				if(!pPoint)
					break;
				strResult.Format(_T("%.1f%s%.1f"), pPoint->x, strSeparator, pPoint->y);
			}
			break;
		default:
			break;
		}


	}
	while(false);

	return strResult;
}


CString CUtilCommon::ConvertRavidLineToString(CRavidGeometry * pGeo, CString strSeparator, CString strDelimiter)
{
	CString strResult;
	strResult.Empty();
	do
	{
		ERavidGeometryTemplateType eType = pGeo->GetTemplateType();

		switch(eType)
		{
		case ERavidGeometryTemplateType_Unknown:
			break;
		case ERavidGeometryTemplateType_Int:
			{
				CRavidLine<int>* pLine = dynamic_cast<CRavidLine<int>*>(pGeo);
				if(!pLine)
					break;
				strResult.Format(_T("%d%s%d%s%d%s%d"),
								 pLine->rpPoints[0].x, strSeparator, pLine->rpPoints[0].y,
								 strDelimiter,
								 pLine->rpPoints[1].x, strSeparator, pLine->rpPoints[1].y);
			}
			break;
		case ERavidGeometryTemplateType_LongLong:
			{
				CRavidLine<long long>* pLine = dynamic_cast<CRavidLine<long long>*>(pGeo);
				if(!pLine)
					break;
				strResult.Format(_T("%lld%s%lld%s%lld%s%lld"),
								 pLine->rpPoints[0].x, strSeparator, pLine->rpPoints[0].y,
								 strDelimiter,
								 pLine->rpPoints[1].x, strSeparator, pLine->rpPoints[1].y);
			}
			break;
		case ERavidGeometryTemplateType_Float:
			{
				CRavidLine<float>* pLine = dynamic_cast<CRavidLine<float>*>(pGeo);
				if(!pLine)
					break;
				strResult.Format(_T("%.1f%s%.1f%s%.1f%s%.1f"),
								 pLine->rpPoints[0].x, strSeparator, pLine->rpPoints[0].y,
								 strDelimiter,
								 pLine->rpPoints[1].x, strSeparator, pLine->rpPoints[1].y);
			}
			break;
		case ERavidGeometryTemplateType_Double:
			{
				CRavidLine<double>* pLine = dynamic_cast<CRavidLine<double>*>(pGeo);
				if(!pLine)
					break;
				strResult.Format(_T("%.1f%s%.1f%s%.1f%s%.1f"),
								 pLine->rpPoints[0].x, strSeparator, pLine->rpPoints[0].y,
								 strDelimiter,
								 pLine->rpPoints[1].x, strSeparator, pLine->rpPoints[1].y);
			}
			break;
		default:
			break;
		}
	}
	while(false);

	return strResult;
}

CString CUtilCommon::ConvertRavidRectToString(CRavidGeometry * pGeo, CString strSeparator)
{
	CString strResult;
	strResult.Empty();
	do
	{
		ERavidGeometryTemplateType eType = pGeo->GetTemplateType();

		switch(eType)
		{
		case ERavidGeometryTemplateType_Unknown:
			break;
		case ERavidGeometryTemplateType_Int:
			{
				CRavidRect<int>* pRect = dynamic_cast<CRavidRect<int>*>(pGeo);
				if(!pRect)
					break;
				strResult.Format(_T("%d%s%d%s%d%s%d"),
								 pRect->left, strSeparator,
								 pRect->top, strSeparator,
								 pRect->right, strSeparator,
								 pRect->bottom);
			}
			break;
		case ERavidGeometryTemplateType_LongLong:
			{
				CRavidRect<long long>* pRect = dynamic_cast<CRavidRect<long long>*>(pGeo);
				if(!pRect)
					break;
				strResult.Format(_T("%lld%s%lld%s%lld%s%lld"),
								 pRect->left, strSeparator,
								 pRect->top, strSeparator,
								 pRect->right, strSeparator,
								 pRect->bottom);
			}
			break;
		case ERavidGeometryTemplateType_Float:
			{
				CRavidRect<float>* pRect = dynamic_cast<CRavidRect<float>*>(pGeo);
				if(!pRect)
					break;
				strResult.Format(_T("%.1f%s%.1f%s%.1f%s%.1f"),
								 pRect->left, strSeparator,
								 pRect->top, strSeparator,
								 pRect->right, strSeparator,
								 pRect->bottom);
			}
			break;
		case ERavidGeometryTemplateType_Double:
			{
				CRavidRect<double>* pRect = dynamic_cast<CRavidRect<double>*>(pGeo);
				if(!pRect)
					break;
				strResult.Format(_T("%.1f%s%.1f%s%.1f%s%.1f"),
								 pRect->left, strSeparator,
								 pRect->top, strSeparator,
								 pRect->right, strSeparator,
								 pRect->bottom);
			}
			break;
		default:
			break;
		}


	}
	while(false);

	return strResult;
}

CString CUtilCommon::ConvertRavidQuadangleToString(CRavidGeometry * pGeo, CString strSeparator, CString strDelimiter)
{
	CString strResult;
	strResult.Empty();
	do
	{
		ERavidGeometryTemplateType eType = pGeo->GetTemplateType();

		CString strPoint;
		int nPoints = 4;
		switch(eType)
		{
		case ERavidGeometryTemplateType_Unknown:
			break;
		case ERavidGeometryTemplateType_Int:
			{
				CRavidQuadrangle<int>* pQuadangle = dynamic_cast<CRavidQuadrangle<int>*>(pGeo);
				if(!pQuadangle)
					break;

				for(int i = 0; i < nPoints; ++i)
				{
					strPoint.Format(_T("%d%s%d"), pQuadangle->rpPoints[i].x, strSeparator, pQuadangle->rpPoints[i].y);
					strResult += strPoint;
					if(i < (nPoints - 1))
						strResult += strDelimiter;
				}
			}
			break;
		case ERavidGeometryTemplateType_LongLong:
			{
				CRavidQuadrangle<long long>* pQuadangle = dynamic_cast<CRavidQuadrangle<long long>*>(pGeo);
				if(!pQuadangle)
					break;

				for(int i = 0; i < nPoints; ++i)
				{
					strPoint.Format(_T("%lld%s%lld"), pQuadangle->rpPoints[i].x, strSeparator, pQuadangle->rpPoints[i].y);
					strResult += strPoint;
					if(i < (nPoints - 1))
						strResult += strDelimiter;
				};
			}
			break;
		case ERavidGeometryTemplateType_Float:
			{
				CRavidQuadrangle<float>* pQuadangle = dynamic_cast<CRavidQuadrangle<float>*>(pGeo);
				if(!pQuadangle)
					break;

				for(int i = 0; i < nPoints; ++i)
				{
					strPoint.Format(_T("%.1f%s%.1f"), pQuadangle->rpPoints[i].x, strSeparator, pQuadangle->rpPoints[i].y);
					strResult += strPoint;
					if(i < (nPoints - 1))
						strResult += strDelimiter;
				}
			}
			break;
		case ERavidGeometryTemplateType_Double:
			{
				CRavidQuadrangle<double>* pQuadangle = dynamic_cast<CRavidQuadrangle<double>*>(pGeo);
				if(!pQuadangle)
					break;

				for(int i = 0; i < nPoints; ++i)
				{
					strPoint.Format(_T("%.1f%s%.1f"), pQuadangle->rpPoints[i].x, strSeparator, pQuadangle->rpPoints[i].y);
					strResult += strPoint;
					if(i < (nPoints - 1))
						strResult += strDelimiter;
				}
			}
			break;
		default:
			break;
		}
	}
	while(false);

	return strResult;
}

CString CUtilCommon::ConvertRavidCircleToString(CRavidGeometry * pGeo, CString strSeparator, CString strDelimiter)
{
	CString strResult;
	strResult.Empty();
	do
	{
		ERavidGeometryTemplateType eType = pGeo->GetTemplateType();

		CString strPoint;
		int nPoints = 4;
		switch(eType)
		{
		case ERavidGeometryTemplateType_Unknown:
			break;
		case ERavidGeometryTemplateType_Int:
			{
				CRavidCircle<int>* pCircle = dynamic_cast<CRavidCircle<int>*>(pGeo);
				if(!pCircle)
					break;

				strResult.Format(_T("%d%s%d%s%d"),
								 pCircle->rpCenter.x, strSeparator, pCircle->rpCenter.y,
								 strDelimiter,
								 pCircle->radius);
			}
			break;
		case ERavidGeometryTemplateType_LongLong:
			{
				CRavidCircle<long long>* pCircle = dynamic_cast<CRavidCircle<long long>*>(pGeo);
				if(!pCircle)
					break;

				strResult.Format(_T("%lld%s%lld%s%lld"),
								 pCircle->rpCenter.x, strSeparator, pCircle->rpCenter.y,
								 strDelimiter,
								 pCircle->radius);
			}
			break;
		case ERavidGeometryTemplateType_Float:
			{
				CRavidCircle<float>* pCircle = dynamic_cast<CRavidCircle<float>*>(pGeo);
				if(!pCircle)
					break;

				strResult.Format(_T("%.1f%s%.1f%s%.1f"),
								 pCircle->rpCenter.x, strSeparator, pCircle->rpCenter.y,
								 strDelimiter,
								 pCircle->radius);
			}
			break;
		case ERavidGeometryTemplateType_Double:
			{
				CRavidCircle<double>* pCircle = dynamic_cast<CRavidCircle<double>*>(pGeo);
				if(!pCircle)
					break;

				strResult.Format(_T("%.1f%s%.1f%s%.1f"),
								 pCircle->rpCenter.x, strSeparator, pCircle->rpCenter.y,
								 strDelimiter,
								 pCircle->radius);
			}
			break;
		default:
			break;
		}
	}
	while(false);

	return strResult;
}


CString CUtilCommon::ConvertRavidEllipseToString(CRavidGeometry * pGeo, CString strSeparator, CString strDelimiter)
{
	CString strResult;
	strResult.Empty();
	do
	{
		ERavidGeometryTemplateType eType = pGeo->GetTemplateType();

		CString strPoint;
		int nPoints = 4;
		switch(eType)
		{
		case ERavidGeometryTemplateType_Unknown:
			break;
		case ERavidGeometryTemplateType_Int:
			{
				CRavidEllipse<int>* pEllipse = dynamic_cast<CRavidEllipse<int>*>(pGeo);
				if(!pEllipse)
					break;

				strResult.Format(_T("%d%s%d%s%d%s%d%s%.3f%s%.3f%s%.3f"),
								 pEllipse->rpCenter.x, strSeparator, pEllipse->rpCenter.y,
								 strDelimiter,
								 pEllipse->radius1,
								 strDelimiter,
								 pEllipse->radius2,
								 strDelimiter,
								 pEllipse->angle,
								 strDelimiter,
								 pEllipse->startDeg,
								 strDelimiter,
								 pEllipse->deltaDeg);
			}
			break;
		case ERavidGeometryTemplateType_LongLong:
			{
				CRavidEllipse<long long>* pEllipse = dynamic_cast<CRavidEllipse<long long>*>(pGeo);
				if(!pEllipse)
					break;

				strResult.Format(_T("%lld%s%lld%s%lld%s%lld%s%.3f%s%.3f%s%.3f"),
								 pEllipse->rpCenter.x, strSeparator, pEllipse->rpCenter.y,
								 strDelimiter,
								 pEllipse->radius1,
								 strDelimiter,
								 pEllipse->radius2,
								 strDelimiter,
								 pEllipse->angle,
								 strDelimiter,
								 pEllipse->startDeg,
								 strDelimiter,
								 pEllipse->deltaDeg);
			}
			break;
		case ERavidGeometryTemplateType_Float:
			{
				CRavidEllipse<float>* pEllipse = dynamic_cast<CRavidEllipse<float>*>(pGeo);
				if(!pEllipse)
					break;

				strResult.Format(_T("%.3f%s%.3f%s%.3f%s%.3f%s%.3f%s%.3f%s%.3f"),
								 pEllipse->rpCenter.x, strSeparator, pEllipse->rpCenter.y,
								 strDelimiter,
								 pEllipse->radius1,
								 strDelimiter,
								 pEllipse->radius2,
								 strDelimiter,
								 pEllipse->angle,
								 strDelimiter,
								 pEllipse->startDeg,
								 strDelimiter,
								 pEllipse->deltaDeg);
			}
			break;
		case ERavidGeometryTemplateType_Double:
			{
				CRavidEllipse<double>* pEllipse = dynamic_cast<CRavidEllipse<double>*>(pGeo);
				if(!pEllipse)
					break;

				strResult.Format(_T("%.3f%s%.3f%s%.3f%s%.3f%s%.3f%s%.3f%s%.3f"),
								 pEllipse->rpCenter.x, strSeparator, pEllipse->rpCenter.y,
								 strDelimiter,
								 pEllipse->radius1,
								 strDelimiter,
								 pEllipse->radius2,
								 strDelimiter,
								 pEllipse->angle,
								 strDelimiter,
								 pEllipse->startDeg,
								 strDelimiter,
								 pEllipse->deltaDeg);
			}
			break;
		default:
			break;
		}
	}
	while(false);

	return strResult;
}

CString CUtilCommon::ConvertRavidPolygonToString(CRavidGeometryArray * pGeoArray, CString strSeparator, CString strDelimiter, CString strShapeSeparator, CString strArraySeparator)
{
	CString strResult;
	strResult.Empty();
	do
	{
		int nArray = pGeoArray->GetCount();

		CString strShape;

		for(int i = 0; i < nArray; ++i)
		{
			ERavidGeometryShapeType eType = pGeoArray->GetAt(i)->GetShapeType();

			strShape.Empty();
			strShape.Format(_T("%d%s"), eType, strShapeSeparator);

			switch(eType)
			{
			case ERavidGeometryShapeType_Unknown:
				break;
			case ERavidGeometryShapeType_Point:
				break;
			case ERavidGeometryShapeType_Line:
				strResult += strShape;
				strResult += ConvertRavidLineToString(pGeoArray->GetAt(i), strSeparator, strDelimiter);
				break;
			case ERavidGeometryShapeType_Rectangle:
				break;
			case ERavidGeometryShapeType_Quadrangle:
				break;
			case ERavidGeometryShapeType_Circle:
				break;
			case ERavidGeometryShapeType_Ellipse:
				strResult += strShape;
				strResult += ConvertRavidEllipseToString(pGeoArray->GetAt(i), strSeparator, strDelimiter);
				break;
			case ERavidGeometryShapeType_Polygon:
				break;
			case ERavidGeometryShapeType_Array:
				break;
			default:
				break;
			}

			if(i < (nArray - 1))
				strResult += strArraySeparator;
		}

	}
	while(false);

	return strResult;
}


bool CUtilCommon::ConvertStringToRavidPoint(CString strParam, CString strSeprator, CRavidPoint<int>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strSeprator) != 2)
			break;

		if(sm.GetSplit(1).IsEmpty())
			break;

		pResult->x = _ttol(sm.GetSplit(0));
		pResult->y = _ttol(sm.GetSplit(1));

		bReturn = true;

	}
	while(false);

	return bReturn;
}


bool CUtilCommon::ConvertStringToRavidPoint(CString strParam, CString strSeprator, CRavidPoint<long long>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strSeprator) != 2)
			break;

		if(sm.GetSplit(1).IsEmpty())
			break;

		pResult->x = _ttoll(sm.GetSplit(0));
		pResult->y = _ttoll(sm.GetSplit(1));

		bReturn = true;

	}
	while(false);

	return bReturn;
}



bool CUtilCommon::ConvertStringToRavidPoint(CString strParam, CString strSeprator, CRavidPoint<float>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strSeprator) != 2)
			break;

		if(sm.GetSplit(1).IsEmpty())
			break;

		pResult->x = (float)_ttof(sm.GetSplit(0));
		pResult->y = (float)_ttof(sm.GetSplit(1));



		bReturn = true;

	}
	while(false);

	return bReturn;
}



bool CUtilCommon::ConvertStringToRavidPoint(CString strParam, CString strSeprator, CRavidPoint<double>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strSeprator) != 2)
			break;

		if(sm.GetSplit(1).IsEmpty())
			break;

		pResult->x = _ttof(sm.GetSplit(0));
		pResult->y = _ttof(sm.GetSplit(1));

		bReturn = true;

	}
	while(false);

	return bReturn;
}



bool CUtilCommon::ConvertStringToRavidLine(CString strParam, CString strSeprator, CString strDelimiter, CRavidLine<int>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(strDelimiter.IsEmpty())
			break;
		if(!pResult)
			break;

		int nPoint = 2;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nPoint)
			break;

		if(sm.GetSplit(nPoint - 1).IsEmpty())
			break;

		bReturn = true;

		for(int i = 0; i < nPoint; ++i)
		{
			bReturn &= ConvertStringToRavidPoint(sm.GetSplit(i), strSeprator, &pResult->rpPoints[i]);
		}

	}
	while(false);

	return bReturn;
}



bool CUtilCommon::ConvertStringToRavidLine(CString strParam, CString strSeprator, CString strDelimiter, CRavidLine<long long>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(strDelimiter.IsEmpty())
			break;
		if(!pResult)
			break;

		int nPoint = 2;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nPoint)
			break;

		if(sm.GetSplit(nPoint - 1).IsEmpty())
			break;

		bReturn = true;

		for(int i = 0; i < nPoint; ++i)
		{
			bReturn &= ConvertStringToRavidPoint(sm.GetSplit(i), strSeprator, &pResult->rpPoints[i]);
		}

	}
	while(false);

	return bReturn;
}


bool CUtilCommon::ConvertStringToRavidLine(CString strParam, CString strSeprator, CString strDelimiter, CRavidLine<float>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(strDelimiter.IsEmpty())
			break;
		if(!pResult)
			break;

		int nPoint = 2;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nPoint)
			break;

		if(sm.GetSplit(nPoint - 1).IsEmpty())
			break;

		bReturn = true;

		for(int i = 0; i < nPoint; ++i)
		{
			bReturn &= ConvertStringToRavidPoint(sm.GetSplit(i), strSeprator, &pResult->rpPoints[i]);
		}

	}
	while(false);

	return bReturn;
}


bool CUtilCommon::ConvertStringToRavidLine(CString strParam, CString strSeprator, CString strDelimiter, CRavidLine<double>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(strDelimiter.IsEmpty())
			break;
		if(!pResult)
			break;

		int nPoint = 2;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nPoint)
			break;

		if(sm.GetSplit(nPoint - 1).IsEmpty())
			break;

		bReturn = true;

		for(int i = 0; i < nPoint; ++i)
		{
			bReturn &= ConvertStringToRavidPoint(sm.GetSplit(i), strSeprator, &pResult->rpPoints[i]);
		}

	}
	while(false);

	return bReturn;
}

bool CUtilCommon::ConvertStringToRavidRect(CString strParam, CString strSeprator, CRavidRect<int>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strSeprator) != 4)
			break;

		if(sm.GetSplit(3).IsEmpty())
			break;

		pResult->left = _ttol(sm.GetSplit(0));
		pResult->top = _ttol(sm.GetSplit(1));
		pResult->right = _ttol(sm.GetSplit(2));
		pResult->bottom = _ttol(sm.GetSplit(3));

		bReturn = true;

	}
	while(false);

	return bReturn;
}

bool CUtilCommon::ConvertStringToRavidRect(CString strParam, CString strSeprator, CRavidRect<long long>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strSeprator) != 4)
			break;

		if(sm.GetSplit(3).IsEmpty())
			break;

		pResult->left = _ttoll(sm.GetSplit(0));
		pResult->top = _ttoll(sm.GetSplit(1));
		pResult->right = _ttoll(sm.GetSplit(2));
		pResult->bottom = _ttoll(sm.GetSplit(3));

		bReturn = true;

	}
	while(false);

	return bReturn;
}


bool CUtilCommon::ConvertStringToRavidRect(CString strParam, CString strSeprator, CRavidRect<float>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strSeprator) != 4)
			break;

		if(sm.GetSplit(3).IsEmpty())
			break;

		pResult->left = (float)_ttof(sm.GetSplit(0));
		pResult->top = (float)_ttof(sm.GetSplit(1));
		pResult->right = (float)_ttof(sm.GetSplit(2));
		pResult->bottom = (float)_ttof(sm.GetSplit(3));

		bReturn = true;

	}
	while(false);

	return bReturn;
}


bool CUtilCommon::ConvertStringToRavidRect(CString strParam, CString strSeprator, CRavidRect<double>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strSeprator) != 4)
			break;

		if(sm.GetSplit(3).IsEmpty())
			break;

		pResult->left = _ttof(sm.GetSplit(0));
		pResult->top = _ttof(sm.GetSplit(1));
		pResult->right = _ttof(sm.GetSplit(2));
		pResult->bottom = _ttof(sm.GetSplit(3));

		bReturn = true;

	}
	while(false);

	return bReturn;
}

bool CUtilCommon::ConvertStringToRavidQuadangle(CString strParam, CString strSeprator, CString strDelimiter, CRavidQuadrangle<int>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(strDelimiter.IsEmpty())
			break;
		if(!pResult)
			break;

		int nPoint = 4;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nPoint)
			break;

		if(sm.GetSplit(nPoint - 1).IsEmpty())
			break;

		bReturn = true;

		for(int i = 0; i < nPoint; ++i)
		{
			bReturn &= ConvertStringToRavidPoint(sm.GetSplit(i), strSeprator, &pResult->rpPoints[i]);
		}

	}
	while(false);

	return bReturn;
}


bool CUtilCommon::ConvertStringToRavidQuadangle(CString strParam, CString strSeprator, CString strDelimiter, CRavidQuadrangle<long long>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(strDelimiter.IsEmpty())
			break;
		if(!pResult)
			break;

		int nPoint = 4;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nPoint)
			break;

		if(sm.GetSplit(nPoint - 1).IsEmpty())
			break;

		bReturn = true;

		for(int i = 0; i < nPoint; ++i)
		{
			bReturn &= ConvertStringToRavidPoint(sm.GetSplit(i), strSeprator, &pResult->rpPoints[i]);
		}

	}
	while(false);

	return bReturn;
}


bool CUtilCommon::ConvertStringToRavidQuadangle(CString strParam, CString strSeprator, CString strDelimiter, CRavidQuadrangle<float>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(strDelimiter.IsEmpty())
			break;
		if(!pResult)
			break;

		int nPoint = 4;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nPoint)
			break;

		if(sm.GetSplit(nPoint - 1).IsEmpty())
			break;

		bReturn = true;

		for(int i = 0; i < nPoint; ++i)
		{
			bReturn &= ConvertStringToRavidPoint(sm.GetSplit(i), strSeprator, &pResult->rpPoints[i]);
		}

	}
	while(false);

	return bReturn;
}


bool CUtilCommon::ConvertStringToRavidQuadangle(CString strParam, CString strSeprator, CString strDelimiter, CRavidQuadrangle<double>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(strDelimiter.IsEmpty())
			break;
		if(!pResult)
			break;

		int nPoint = 4;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nPoint)
			break;

		if(sm.GetSplit(nPoint - 1).IsEmpty())
			break;

		bReturn = true;

		for(int i = 0; i < nPoint; ++i)
		{
			bReturn &= ConvertStringToRavidPoint(sm.GetSplit(i), strSeprator, &pResult->rpPoints[i]);
		}

	}
	while(false);

	return bReturn;
}

bool CUtilCommon::ConvertStringToRavidCircle(CString strParam, CString strSeprator, CString strDelimiter, CRavidCircle<int>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		int nCount = 2;
		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nCount)
			break;

		if(sm.GetSplit((nCount - 1)).IsEmpty())
			break;

		// P1 / radius....
		CUtilStringManager sm2;
		if(sm2.SetSplit(sm.GetSplit(0), strSeprator) != 2)
			break;
		if(sm2.GetSplit(1).IsEmpty())
			break;

		pResult->rpCenter.x = _ttol(sm2.GetSplit(0));
		pResult->rpCenter.y = _ttol(sm2.GetSplit(1));

		pResult->radius = _ttol(sm.GetSplit(1));

		bReturn = true;

	}
	while(false);

	return bReturn;
}


bool CUtilCommon::ConvertStringToRavidCircle(CString strParam, CString strSeprator, CString strDelimiter, CRavidCircle<long long>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		int nCount = 2;
		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nCount)
			break;

		if(sm.GetSplit((nCount - 1)).IsEmpty())
			break;

		// P1 / radius....
		CUtilStringManager sm2;
		if(sm2.SetSplit(sm.GetSplit(0), strSeprator) != 2)
			break;
		if(sm2.GetSplit(1).IsEmpty())
			break;

		pResult->rpCenter.x = _ttoll(sm2.GetSplit(0));
		pResult->rpCenter.y = _ttoll(sm2.GetSplit(1));

		pResult->radius = _ttoll(sm.GetSplit(1));

		bReturn = true;

	}
	while(false);

	return bReturn;
}



bool CUtilCommon::ConvertStringToRavidCircle(CString strParam, CString strSeprator, CString strDelimiter, CRavidCircle<float>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		int nCount = 2;
		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nCount)
			break;

		if(sm.GetSplit((nCount - 1)).IsEmpty())
			break;

		// P1 / radius....
		CUtilStringManager sm2;
		if(sm2.SetSplit(sm.GetSplit(0), strSeprator) != 2)
			break;
		if(sm2.GetSplit(1).IsEmpty())
			break;

		pResult->rpCenter.x = (float)_ttof(sm2.GetSplit(0));
		pResult->rpCenter.y = (float)_ttof(sm2.GetSplit(1));

		pResult->radius = (float)_ttof(sm.GetSplit(1));

		bReturn = true;

	}
	while(false);

	return bReturn;
}


bool CUtilCommon::ConvertStringToRavidCircle(CString strParam, CString strSeprator, CString strDelimiter, CRavidCircle<double>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		int nCount = 2;
		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nCount)
			break;

		if(sm.GetSplit((nCount - 1)).IsEmpty())
			break;

		// P1 / radius....
		CUtilStringManager sm2;
		if(sm2.SetSplit(sm.GetSplit(0), strSeprator) != 2)
			break;
		if(sm2.GetSplit(1).IsEmpty())
			break;

		pResult->rpCenter.x = _ttof(sm2.GetSplit(0));
		pResult->rpCenter.y = _ttof(sm2.GetSplit(1));

		pResult->radius = _ttof(sm.GetSplit(1));

		bReturn = true;

	}
	while(false);

	return bReturn;
}

bool CUtilCommon::ConvertStringToRavidEllipse(CString strParam, CString strSeprator, CString strDelimiter, CRavidEllipse<int>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		// P1 / radius1 / radius2 / angle / startDeg / deltaDeg
		int nCount = 6;
		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nCount)
			break;

		if(sm.GetSplit((nCount - 1)).IsEmpty())
			break;

		CUtilStringManager sm2;
		if(sm2.SetSplit(sm.GetSplit(0), strSeprator) != 2)
			break;
		if(sm2.GetSplit(1).IsEmpty())
			break;

		pResult->rpCenter.x = _ttol(sm2.GetSplit(0));
		pResult->rpCenter.y = _ttol(sm2.GetSplit(1));

		pResult->radius1 = _ttol(sm.GetSplit(1));
		pResult->radius2 = _ttol(sm.GetSplit(2));

		pResult->angle = _ttof(sm.GetSplit(3));
		pResult->startDeg = _ttof(sm.GetSplit(4));
		pResult->deltaDeg = _ttof(sm.GetSplit(5));

		bReturn = true;

	}
	while(false);

	return bReturn;
}

bool CUtilCommon::ConvertStringToRavidEllipse(CString strParam, CString strSeprator, CString strDelimiter, CRavidEllipse<long long>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		// P1 / radius1 / radius2 / angle / startDeg / deltaDeg
		int nCount = 6;
		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nCount)
			break;

		if(sm.GetSplit((nCount - 1)).IsEmpty())
			break;

		CUtilStringManager sm2;
		if(sm2.SetSplit(sm.GetSplit(0), strSeprator) != 2)
			break;
		if(sm2.GetSplit(1).IsEmpty())
			break;

		pResult->rpCenter.x = _ttoll(sm2.GetSplit(0));
		pResult->rpCenter.y = _ttoll(sm2.GetSplit(1));

		pResult->radius1 = _ttoll(sm.GetSplit(1));
		pResult->radius2 = _ttoll(sm.GetSplit(2));

		pResult->angle = _ttof(sm.GetSplit(3));
		pResult->startDeg = _ttof(sm.GetSplit(4));
		pResult->deltaDeg = _ttof(sm.GetSplit(5));

		bReturn = true;

	}
	while(false);

	return bReturn;
}


bool CUtilCommon::ConvertStringToRavidEllipse(CString strParam, CString strSeprator, CString strDelimiter, CRavidEllipse<float>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		// P1 / radius1 / radius2 / angle / startDeg / deltaDeg
		int nCount = 6;
		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nCount)
			break;

		if(sm.GetSplit((nCount - 1)).IsEmpty())
			break;

		CUtilStringManager sm2;
		if(sm2.SetSplit(sm.GetSplit(0), strSeprator) != 2)
			break;
		if(sm2.GetSplit(1).IsEmpty())
			break;

		pResult->rpCenter.x = (float)_ttof(sm2.GetSplit(0));
		pResult->rpCenter.y = (float)_ttof(sm2.GetSplit(1));

		pResult->radius1 = (float)_ttof(sm.GetSplit(1));
		pResult->radius2 = (float)_ttof(sm.GetSplit(2));

		pResult->angle = _ttof(sm.GetSplit(3));
		pResult->startDeg = _ttof(sm.GetSplit(4));
		pResult->deltaDeg = _ttof(sm.GetSplit(5));

		bReturn = true;

	}
	while(false);

	return bReturn;
}



bool CUtilCommon::ConvertStringToRavidEllipse(CString strParam, CString strSeprator, CString strDelimiter, CRavidEllipse<double>* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeprator.IsEmpty())
			break;
		if(!pResult)
			break;

		// P1 / radius1 / radius2 / angle / startDeg / deltaDeg
		int nCount = 6;
		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strDelimiter) != nCount)
			break;

		if(sm.GetSplit((nCount - 1)).IsEmpty())
			break;

		CUtilStringManager sm2;
		if(sm2.SetSplit(sm.GetSplit(0), strSeprator) != 2)
			break;
		if(sm2.GetSplit(1).IsEmpty())
			break;

		pResult->rpCenter.x = _ttof(sm2.GetSplit(0));
		pResult->rpCenter.y = _ttof(sm2.GetSplit(1));

		pResult->radius1 = _ttof(sm.GetSplit(1));
		pResult->radius2 = _ttof(sm.GetSplit(2));

		pResult->angle = _ttof(sm.GetSplit(3));
		pResult->startDeg = _ttof(sm.GetSplit(4));
		pResult->deltaDeg = _ttof(sm.GetSplit(5));

		bReturn = true;

	}
	while(false);

	return bReturn;
}

bool CUtilCommon::ConvertStringToRavidPolygon(CString strParam, CString strSeparator, CString strDelimiter, CString strShapeSeparator, CString strArraySeparator, CRavidGeometryArray * pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeparator.IsEmpty())
			break;
		if(strDelimiter.IsEmpty())
			break;
		if(strShapeSeparator.IsEmpty())
			break;
		if(strArraySeparator.IsEmpty())
			break;
		if(!pResult)
			break;

		CUtilStringManager sm1;
		int nArray = (int)sm1.SetSplit(strParam, strArraySeparator);
		if(!nArray)
			break;

		for(int i = 0; i < nArray; ++i)
		{
			CUtilStringManager sm2;
			if(sm2.SetSplit(sm1.GetSplit(i), strShapeSeparator) != 2)
				break;
			if(sm2.GetSplit(1).IsEmpty())
				break;

			int nShape = _ttol(sm2.GetSplit(0));

			switch(nShape)
			{
			case ERavidGeometryShapeType_Unknown:
				break;
			case ERavidGeometryShapeType_Point:
				break;
			case ERavidGeometryShapeType_Line:
				{
					CRavidLine<double> rLine;
					if(ConvertStringToRavidLine(sm2.GetSplit(1), strSeparator, strDelimiter, &rLine))
					{
						pResult->Add(&rLine);
					}
				}
				break;
			case ERavidGeometryShapeType_Rectangle:
				break;
			case ERavidGeometryShapeType_Quadrangle:
				break;
			case ERavidGeometryShapeType_Circle:
				break;
			case ERavidGeometryShapeType_Ellipse:
				{
					CRavidEllipse<double> rE;
					if(ConvertStringToRavidEllipse(sm2.GetSplit(1), strSeparator, strDelimiter, &rE))
					{
						pResult->Add(&rE);
					}
				}
				break;
			case ERavidGeometryShapeType_Polygon:
				break;
			case ERavidGeometryShapeType_Array:
				break;
			default:
				break;
			}
		}


		if(!pResult->GetCount())
			break;


		bReturn = true;

	}
	while(false);

	return bReturn;
}

bool CUtilCommon::ConvertStringToDouble(CString strParam, CString strSeparator, int nTotalCount, double* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeparator.IsEmpty())
			break;
		if(!pResult)
			break;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strSeparator) < 1)
			break;

		int nCount = sm.GetCount();

		if(nCount <= nTotalCount)
		{
			for(int i = 0; i < nCount; i++)
				pResult[i] = _ttof(sm.GetSplit(i));

			for(int i = nCount; i < nTotalCount; i++)
				pResult[i] = 0;
		}
		else
		{
			for(int i = 0; i < nTotalCount; i++)
				pResult[i] = _ttof(sm.GetSplit(i));
		}

		bReturn = true;

	}
	while(false);

	return bReturn;
}

bool CUtilCommon::ConvertStringToInt(CString strParam, CString strSeparator, int nTotalCount, int* pResult)
{
	bool bReturn = false;
	do
	{
		if(strParam.IsEmpty())
			break;
		if(strSeparator.IsEmpty())
			break;
		if(!pResult)
			break;

		CUtilStringManager sm;
		if(sm.SetSplit(strParam, strSeparator) < 1)
			break;

		int nCount = sm.GetCount();

		if(nCount <= nTotalCount)
		{
			for(int i = 0; i < nCount; i++)
				pResult[i] = _ttoi(sm.GetSplit(i));

			for(int i = nCount; i < nTotalCount; i++)
				pResult[i] = 0;
		}
		else
		{
			for(int i = 0; i < nTotalCount; i++)
				pResult[i] = _ttoi(sm.GetSplit(i));
		}

		bReturn = true;

	}
	while(false);

	return bReturn;
}
bool CUtilCommon::ConvertStringToString(CString strParam, CString strSeparator, int nTotalCount, CString* pResult)
{
	bool bReturn = false;
	do
	{
		CUtilStringManager sm;
		sm.SetSplit(strParam, strSeparator);
		int nCount = sm.GetCount();

		if(nCount <= nTotalCount)
		{
			for(int i = 0; i < nCount; i++)
				pResult[i] = sm.GetSplit(i);

			for(int i = nCount; i < nTotalCount; i++)
				pResult[i] = _T("1");
		}
		else
		{
			for(int i = 0; i < nTotalCount; i++)
				pResult[i] = sm.GetSplit(i);
		}

		bReturn = true;

	}
	while(false);

	return bReturn;
}

bool CUtilCommon::ConvertStringToString(CString strParam, CString strSeparator, int nTotalCount, CString strInit, CString* pResult)
{
	bool bReturn = false;
	do
	{
		CUtilStringManager sm;
		sm.SetSplit(strParam, strSeparator);
		int nCount = sm.GetCount();

		if(nCount <= nTotalCount)
		{
			for(int i = 0; i < nCount; i++)
				pResult[i] = sm.GetSplit(i);

			for(int i = nCount; i < nTotalCount; i++)
				pResult[i] = strInit;
		}
		else
		{
			for(int i = 0; i < nTotalCount; i++)
				pResult[i] = sm.GetSplit(i);
		}

		bReturn = true;

	}
	while(false);

	return bReturn;
}
bool CUtilCommon::ConvertStringToVctPoints(CString strParam, CString strSeparator, std::vector<CRavidPoint<double>>* pResult)
{
	bool bReturn = false;
	do
	{
		if(!pResult)
			break;

		pResult->clear();

		CUtilStringManager sm;
		sm.SetSplit(strParam, strSeparator);
		int nCount = sm.GetCount();

		for(int i = 0; i < nCount; i++)
		{
			CRavidPoint<double> rp;
			ConvertStringToRavidPoint(sm.GetSplit(i), _T("//"), &rp);
			pResult->push_back(rp);
		}


		bReturn = true;

	}
	while(false);

	return bReturn;
}

const CString & CUtilCommon::GetRootPath()
{
	// TODO: 여기에 반환 구문을 삽입합니다.

	static CString strTemp;

	if(strTemp.IsEmpty())
	{
		GetModuleFileName(NULL, strTemp.GetBuffer(MAX_PATH), MAX_PATH);
		strTemp.ReleaseBuffer();
		strTemp = strTemp.Left(strTemp.ReverseFind('\\'));
		strTemp = strTemp.Left(strTemp.ReverseFind('\\'));
	}

	return strTemp;
}

//////////////////////////////////////////////////////////////////////////
//범용 구조체 -> 스트링 , 스트링 -> 구조체
CRavidGeometry* CUtilCommon::ConvertStringToRavidGeometry(CString strParam)
{
	CRavidGeometry* bReturn = nullptr;
	do
	{
		if(strParam.IsEmpty())
			break;

		bool bCurrentObject = true;
		CString strMsg;
		strMsg.Empty();

		CString strGeoType;
		CString strGeo;
		CString strSep;
		CStringArray strAdd;
		strAdd.RemoveAll();
		strGeoType.Empty();
		strGeo.Empty();
		strSep.Empty();

		CString strSource = strParam;
		ERavidGeometryShapeType eType;
		CRavidGeometryArray* pGeoArray = nullptr;

		int nPos = 0;
		strSep.Format(_T("[("));
		int nLenSep = (int)_tcslen(strSep);

		bool bFind = false;

		if((nPos = strSource.Find(strSep)) != -1)
		{
			bFind = true;
			nLenSep = (int)_tcslen(strSep);
			strGeoType.Append(strSource.Left(nPos));
			strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
		}
		else if((nPos = strSource.Find(_T("[+"))) != -1)
		{
			bFind = true;
			nLenSep = (int)_tcslen(_T("[+"));
			strGeoType.Append(strSource.Left(nPos));
			strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
		}
		else
		{
			bFind = false;
		}

		if(!bFind)
			break;

		if(strGeoType == _T("Point"))
			eType = ERavidGeometryShapeType_Point;
		else if(strGeoType == _T("Line"))
			eType = ERavidGeometryShapeType_Line;
		else if(strGeoType == _T("Rectangle"))
			eType = ERavidGeometryShapeType_Rectangle;
		else if(strGeoType == _T("Quadrangle"))
			eType = ERavidGeometryShapeType_Quadrangle;
		else if(strGeoType == _T("Circle"))
			eType = ERavidGeometryShapeType_Circle;
		else if(strGeoType == _T("Ellipse"))
			eType = ERavidGeometryShapeType_Ellipse;
		else if(strGeoType == _T("Polygon"))
			eType = ERavidGeometryShapeType_Polygon;
		else
			eType = ERavidGeometryShapeType_Unknown;

		strSep.Format(_T(")]"));
		bFind = true;

		if(!strSource.IsEmpty())
		{
			switch(eType)
			{
			case ERavidGeometryShapeType_Unknown:
				bCurrentObject = false;
				break;
			case ERavidGeometryShapeType_Point:
				{
					if((nPos = strSource.Find(strSep)) != -1)
					{
						bFind = true;
						nLenSep = (int)_tcslen(strSep);
						strGeo.Append(strSource.Left(nPos));
						if(!strSource.Right(strSource.GetLength() - (nPos + nLenSep)).IsEmpty())
							strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
					}
					else
					{
						bFind = false;
					}

					if(!bFind)
					{
						break;
					}

					if(!ConvertStringToRavidPoint(strGeo, _T(","), &m_rpPoint))
					{
						break;
					}

					//////////////////////////////////////////////////////////////////////////
					if(strSource == _T("d"))
					{
						bReturn = m_rpPoint.MakeDifferentTemplateType(ERavidGeometryTemplateType_Int);
					}
					else if(strSource == _T("lld"))
					{
						bReturn = m_rpPoint.MakeDifferentTemplateType(ERavidGeometryTemplateType_LongLong);
					}
					else if(strSource == _T("f"))
					{
						bReturn = m_rpPoint.MakeDifferentTemplateType(ERavidGeometryTemplateType_Float);
					}
					else if(strSource == _T("lf"))
					{
						bReturn = m_rpPoint.MakeDifferentTemplateType(ERavidGeometryTemplateType_Double);
					}
					else
						bReturn = nullptr;
					//////////////////////////////////////////////////////////////////////////

					//bReturn = &m_rpPoint;
				}
				break;
			case ERavidGeometryShapeType_Line:
				{
					if((nPos = strSource.Find(strSep)) != -1)
					{
						bFind = true;
						nLenSep = (int)_tcslen(strSep);
						strGeo.Append(strSource.Left(nPos));
						if(!strSource.Right(strSource.GetLength() - (nPos + nLenSep)).IsEmpty())
							strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
					}
					else
					{
						bFind = false;
					}

					if(!bFind)
					{
						break;
					}

					if(!ConvertStringToRavidLine(strGeo, _T(","), _T("/"), &m_rlLine))
					{
						break;
					}

					//////////////////////////////////////////////////////////////////////////
					if(strSource == _T("d"))
					{
						bReturn = m_rlLine.MakeDifferentTemplateType(ERavidGeometryTemplateType_Int);
					}
					else if(strSource == _T("lld"))
					{
						bReturn = m_rlLine.MakeDifferentTemplateType(ERavidGeometryTemplateType_LongLong);
					}
					else if(strSource == _T("f"))
					{
						bReturn = m_rlLine.MakeDifferentTemplateType(ERavidGeometryTemplateType_Float);
					}
					else if(strSource == _T("lf"))
					{
						bReturn = m_rlLine.MakeDifferentTemplateType(ERavidGeometryTemplateType_Double);
					}
					else
						bReturn = nullptr;
					//////////////////////////////////////////////////////////////////////////

					//bReturn = &m_rlLine;
				}
				break;
			case ERavidGeometryShapeType_Rectangle:
				{
					if((nPos = strSource.Find(strSep)) != -1)
					{
						bFind = true;
						nLenSep = (int)_tcslen(strSep);
						strGeo.Append(strSource.Left(nPos));
						if(!strSource.Right(strSource.GetLength() - (nPos + nLenSep)).IsEmpty())
							strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
					}
					else
					{
						bFind = false;
					}

					if(!bFind)
					{
						break;
					}

					if(!ConvertStringToRavidRect(strGeo, _T(","), &m_rrRect))
					{
						break;
					}

					//////////////////////////////////////////////////////////////////////////
					if(strSource == _T("d"))
					{
						bReturn = m_rrRect.MakeDifferentTemplateType(ERavidGeometryTemplateType_Int);
					}
					else if(strSource == _T("lld"))
					{
						bReturn = m_rrRect.MakeDifferentTemplateType(ERavidGeometryTemplateType_LongLong);
					}
					else if(strSource == _T("f"))
					{
						bReturn = m_rrRect.MakeDifferentTemplateType(ERavidGeometryTemplateType_Float);
					}
					else if(strSource == _T("lf"))
					{
						bReturn = m_rrRect.MakeDifferentTemplateType(ERavidGeometryTemplateType_Double);
					}
					else
						bReturn = nullptr;
					//////////////////////////////////////////////////////////////////////////

					//bReturn = &m_rrRect;
				}
				break;
			case ERavidGeometryShapeType_Quadrangle:
				{
					if((nPos = strSource.Find(strSep)) != -1)
					{
						bFind = true;
						nLenSep = (int)_tcslen(strSep);
						strGeo.Append(strSource.Left(nPos));
						if(!strSource.Right(strSource.GetLength() - (nPos + nLenSep)).IsEmpty())
							strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
					}
					else
					{
						bFind = false;
					}

					if(!bFind)
					{
						break;
					}

					if(!ConvertStringToRavidQuadangle(strGeo, _T(","), _T("/"), &m_rqQuadrangle))
					{
						break;
					}

					//////////////////////////////////////////////////////////////////////////
					if(strSource == _T("d"))
					{
						bReturn = m_rqQuadrangle.MakeDifferentTemplateType(ERavidGeometryTemplateType_Int);
					}
					else if(strSource == _T("lld"))
					{
						bReturn = m_rqQuadrangle.MakeDifferentTemplateType(ERavidGeometryTemplateType_LongLong);
					}
					else if(strSource == _T("f"))
					{
						bReturn = m_rqQuadrangle.MakeDifferentTemplateType(ERavidGeometryTemplateType_Float);
					}
					else if(strSource == _T("lf"))
					{
						bReturn = m_rqQuadrangle.MakeDifferentTemplateType(ERavidGeometryTemplateType_Double);
					}
					else
						bReturn = nullptr;
					//////////////////////////////////////////////////////////////////////////

					//bReturn = &m_rqQuadrangle;
				}
				break;
			case ERavidGeometryShapeType_Circle:
				{
					if((nPos = strSource.Find(strSep)) != -1)
					{
						bFind = true;
						nLenSep = (int)_tcslen(strSep);
						strGeo.Append(strSource.Left(nPos));
						if(!strSource.Right(strSource.GetLength() - (nPos + nLenSep)).IsEmpty())
							strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
					}
					else
					{
						bFind = false;
					}

					if(!bFind)
					{
						break;
					}

					if(!ConvertStringToRavidCircle(strGeo, _T(","), _T("/"), &m_rcCircle))
					{
						break;
					}

					//////////////////////////////////////////////////////////////////////////
					if(strSource == _T("d"))
					{
						bReturn = m_rcCircle.MakeDifferentTemplateType(ERavidGeometryTemplateType_Int);
					}
					else if(strSource == _T("lld"))
					{
						bReturn = m_rcCircle.MakeDifferentTemplateType(ERavidGeometryTemplateType_LongLong);
					}
					else if(strSource == _T("f"))
					{
						bReturn = m_rcCircle.MakeDifferentTemplateType(ERavidGeometryTemplateType_Float);
					}
					else if(strSource == _T("lf"))
					{
						bReturn = m_rcCircle.MakeDifferentTemplateType(ERavidGeometryTemplateType_Double);
					}
					else
						bReturn = nullptr;
					//////////////////////////////////////////////////////////////////////////

					//bReturn = &m_rcCircle;
				}
				break;
			case ERavidGeometryShapeType_Ellipse:
				{
					if((nPos = strSource.Find(strSep)) != -1)
					{
						bFind = true;
						nLenSep = (int)_tcslen(strSep);
						strGeo.Append(strSource.Left(nPos));
						if(!strSource.Right(strSource.GetLength() - (nPos + nLenSep)).IsEmpty())
							strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
					}
					else
					{
						bFind = false;
					}

					if(!bFind)
					{
						break;
					}

					if(!ConvertStringToRavidEllipse(strGeo, _T(","), _T("/"), &m_reEllipse))
					{
						break;
					}

					//////////////////////////////////////////////////////////////////////////
					if(strSource == _T("d"))
					{
						bReturn = m_reEllipse.MakeDifferentTemplateType(ERavidGeometryTemplateType_Int);
					}
					else if(strSource == _T("lld"))
					{
						bReturn = m_reEllipse.MakeDifferentTemplateType(ERavidGeometryTemplateType_LongLong);
					}
					else if(strSource == _T("f"))
					{
						bReturn = m_reEllipse.MakeDifferentTemplateType(ERavidGeometryTemplateType_Float);
					}
					else if(strSource == _T("lf"))
					{
						bReturn = m_reEllipse.MakeDifferentTemplateType(ERavidGeometryTemplateType_Double);
					}
					else
						bReturn = nullptr;
					//////////////////////////////////////////////////////////////////////////

					//bReturn = &m_reEllipse;
				}
				break;
			case ERavidGeometryShapeType_Polygon:
				{
					nLenSep = (int)_tcslen(_T("+("));

					m_rPolygon.Clear();

					while(bFind)
					{
						if((nPos = strSource.Find(_T("+("))) != -1)
						{
							nLenSep = (int)_tcslen(_T("+("));
							strAdd.Add(strSource.Left(nPos));
							strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));

							if((nPos = strSource.Find(_T(")+"))) != -1)
							{
								bFind = true;
								nLenSep = (int)_tcslen(_T(")+"));
								strAdd.Add(strSource.Left(nPos));
								strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
							}
							else if((nPos = strSource.Find(_T(")]"))) != -1)
							{
								bFind = true;
								nLenSep = (int)_tcslen(_T(")]"));
								strAdd.Add(strSource.Left(nPos));
								strSource = strSource.Right(strSource.GetLength() - (nPos + nLenSep));
							}
							else
							{
								bFind = false;
							}
						}
						else
						{
							bFind = false;
						}
					}

					for(int i = 0; i < strAdd.GetCount() - 1; i += 2)
					{
						int nShape = _ttoi(strAdd.GetAt(i));
						CString strData = strAdd.GetAt(i + 1);

						switch(nShape)
						{
						case ERavidGeometryShapeType_Unknown:
							break;
						case ERavidGeometryShapeType_Point:
							{
								CRavidPoint<double> rpPoint;
								if(ConvertStringToRavidPoint(strData, _T(","), &rpPoint))
								{
									m_rPolygon.Add(&rpPoint);
								}
							}
							break;
						case ERavidGeometryShapeType_Line:
							{
								CRavidLine<double> rLine;
								if(ConvertStringToRavidLine(strData, _T(","), _T("/"), &rLine))
								{
									m_rPolygon.Add(&rLine);
								}
							}
							break;
						case ERavidGeometryShapeType_Rectangle:
							break;
						case ERavidGeometryShapeType_Quadrangle:
							break;
						case ERavidGeometryShapeType_Circle:
							break;
						case ERavidGeometryShapeType_Ellipse:
							{
								CRavidEllipse<double> rE;
								if(ConvertStringToRavidEllipse(strData, _T(","), _T("/"), &rE))
								{
									m_rPolygon.Add(&rE);
								}
							}
							break;
						case ERavidGeometryShapeType_Polygon:
							break;
						case ERavidGeometryShapeType_Array:
							break;
						default:
							break;
						}
					}

					if(!m_rPolygon.GetCount())
						break;

					CRavidGeometry* pGeoResult;
					pGeoResult = dynamic_cast<CRavidGeometry*>(&m_rPolygon);

					bReturn = pGeoResult;
				}
				break;
			case ERavidGeometryShapeType_Array:
				bCurrentObject = false;
				break;
			default:
				bCurrentObject = false;
				break;
			}
		}

		if(!bCurrentObject)
		{
			strMsg.Format(_T("Teaching Type is Unkown... To Fail..."));
			CLogManager::Write(0, strMsg);

			break;
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
	}
	while(false);

	return bReturn;
}

bool CUtilCommon::ConvertRavidGeometryToString(CRavidGeometry* pGeo, CString& strShape)
{
	bool bReturn = false;

	do
	{
		CString strMsg;
		strMsg.Empty();
		CString strAppend;
		strAppend.Empty();
		bool bCurrentObject = true;

		ERavidGeometryShapeType eType;
		eType = pGeo->GetShapeType();

		//////////////////////////////////////////////////////////////////////////
		ERavidGeometryTemplateType eTemplateType;
		eTemplateType = pGeo->GetTemplateType();
		//////////////////////////////////////////////////////////////////////////

		CRavidGeometryArray* pGeoArray = nullptr;

		switch(eType)
		{
		case ERavidGeometryShapeType_Unknown:
			bCurrentObject = false;
			break;
		case ERavidGeometryShapeType_Point:
			{
				strShape.Empty();
				strShape.Append(_T("Point["));

				strAppend.Empty();
				strAppend.Format(_T("("));
				strShape.Append(strAppend);
				strShape.Append(ConvertRavidPointToString(pGeo, _T(",")));
				strAppend.Format(_T(")"));
				strShape.Append(strAppend);
			}
			break;
		case ERavidGeometryShapeType_Line:
			{
				strShape.Empty();
				strShape.Append(_T("Line["));

				strAppend.Empty();
				strAppend.Format(_T("("));
				strShape.Append(strAppend);
				strShape.Append(ConvertRavidLineToString(pGeo, _T(","), _T("/")));
				strAppend.Format(_T(")"));
				strShape.Append(strAppend);
			}
			break;
		case ERavidGeometryShapeType_Rectangle:
			{
				strShape.Empty();
				strShape.Append(_T("Rectangle["));

				strAppend.Empty();
				strAppend.Format(_T("("));
				strShape.Append(strAppend);
				strShape.Append(ConvertRavidRectToString(pGeo, _T(",")));
				strAppend.Format(_T(")"));
				strShape.Append(strAppend);
			}
			break;
		case ERavidGeometryShapeType_Quadrangle:
			{
				strShape.Empty();
				strShape.Append(_T("Quadrangle["));

				strAppend.Empty();
				strAppend.Format(_T("("));
				strShape.Append(strAppend);
				strShape.Append(ConvertRavidQuadangleToString(pGeo, _T(","), _T("/")));
				strAppend.Format(_T(")"));
				strShape.Append(strAppend);
			}
			break;
		case ERavidGeometryShapeType_Circle:
			{
				strShape.Empty();
				strShape.Append(_T("Circle["));

				strAppend.Empty();
				strAppend.Format(_T("("));
				strShape.Append(strAppend);
				strShape.Append(ConvertRavidCircleToString(pGeo, _T(","), _T("/")));
				strAppend.Format(_T(")"));
				strShape.Append(strAppend);
			}
			break;
		case ERavidGeometryShapeType_Ellipse:
			{
				strShape.Empty();
				strShape.Append(_T("Ellipse["));

				strAppend.Empty();
				strAppend.Format(_T("("));
				strShape.Append(strAppend);
				strShape.Append(ConvertRavidEllipseToString(pGeo, _T(","), _T("/")));
				strAppend.Format(_T(")"));
				strShape.Append(strAppend);
			}
			break;
		case ERavidGeometryShapeType_Polygon:
			{
				pGeoArray = dynamic_cast<CRavidGeometryArray*>(pGeo);

				int nArrCount = pGeoArray->GetCount();
				strShape.Empty();
				strShape.Append(_T("Polygon["));
				for(int i = 0; i < nArrCount; ++i)
				{
					ERavidGeometryShapeType eType = pGeoArray->GetAt(i)->GetShapeType();

					CString strAppend;
					strAppend.Empty();
					strAppend.Format(_T("+%d+"), eType);
					strShape.Append(strAppend);

					switch(eType)
					{
					case ERavidGeometryShapeType_Unknown:
						break;
					case ERavidGeometryShapeType_Point:
						{
							strAppend.Empty();
							strAppend.Format(_T("("));
							strShape.Append(strAppend);
							strShape.Append(ConvertRavidPointToString(pGeoArray->GetAt(i), _T(",")));
							strAppend.Format(_T(")"));
							strShape.Append(strAppend);
						}
						break;
					case ERavidGeometryShapeType_Line:
						{
							strAppend.Empty();
							strAppend.Format(_T("("));
							strShape.Append(strAppend);
							strShape.Append(ConvertRavidLineToString(pGeoArray->GetAt(i), _T(","), _T("/")));
							strAppend.Format(_T(")"));
							strShape.Append(strAppend);
						}
						break;
					case ERavidGeometryShapeType_Rectangle:
						break;
					case ERavidGeometryShapeType_Quadrangle:
						break;
					case ERavidGeometryShapeType_Circle:
						break;
					case ERavidGeometryShapeType_Ellipse:
						{
							strAppend.Empty();
							strAppend.Format(_T("("));
							strShape.Append(strAppend);
							strAppend.Empty();
							strAppend = ConvertRavidEllipseToString(pGeoArray->GetAt(i), _T(","), _T("/"));
							strShape.Append(strAppend);
							strAppend.Format(_T(")"));
							strShape.Append(strAppend);
						}
						break;
					case ERavidGeometryShapeType_Polygon:
						break;
					case ERavidGeometryShapeType_Array:
						break;
					default:
						break;
					}
				}
				//strShape.Append(_T("]"));
			}
			break;
		case ERavidGeometryShapeType_Array:
			bCurrentObject = false;
			break;
		default:
			bCurrentObject = false;
			break;
		}

		strShape.Append(_T("]"));

		//////////////////////////////////////////////////////////////////////////
		switch(eTemplateType)
		{
		case ERavidGeometryTemplateType_Unknown:
			strShape.Append(_T("lf"));
			break;
		case ERavidGeometryTemplateType_Int:
			strShape.Append(_T("d"));
			break;
		case ERavidGeometryTemplateType_LongLong:
			strShape.Append(_T("lld"));
			break;
		case ERavidGeometryTemplateType_Float:
			strShape.Append(_T("f"));
			break;
		case ERavidGeometryTemplateType_Double:
			strShape.Append(_T("lf"));
			break;
		default:
			bCurrentObject = false;
			break;
		}
		//////////////////////////////////////////////////////////////////////////

		if(!bCurrentObject)
		{
			strMsg.Format(_T("Teaching Type is Unkown... To Fail..."));
			CLogManager::Write(0, strMsg);
			break;
		}
	}
	while(false);

	return bReturn;
}