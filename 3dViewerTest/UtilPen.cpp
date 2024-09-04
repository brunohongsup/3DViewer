#include "stdafx.h"
#include "UtilPen.h"


CUtilPen::CUtilPen()
{
	CSingletonCheck* cSingle;
	cSingle = CSingletonCheck::GetInstance();
	m_bUsePen = false;
	m_bLButtonClicking = false;
	m_bMButtonClicking = false;
	cSingle->m_bSelectGV = false;
	cSingle->m_nSelectGV = 0;
	m_vctMPoint.clear();

}

CUtilPen::~CUtilPen()
{
}
void CUtilPen::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);
}

void CUtilPen::OnMouseMove(UINT nFlags, CPoint point)
{
	if (IsLButtonClicking() && IsUsePen())
	{
		CSingletonCheck* cSingle;
		cSingle = CSingletonCheck::GetInstance();

		CRavidPoint<int> rpTemp = point;
		rpTemp = ScreenCoordToImageCoord(rpTemp);
		CRavidPoint<double> rpViewPoint = rpTemp;
		double dblPixel = GetImageInfo()->GetGVBilinear(rpViewPoint);

		if (GetImageInfo()->IsColor())
		{
			GetImageInfo()->SetValue((int)rpViewPoint.x, (int)rpViewPoint.y, (BYTE)cSingle->m_sColor.bRed, (BYTE)cSingle->m_sColor.bGreen, (BYTE)cSingle->m_sColor.bBlue);
		}
		else
		{
			if (cSingle->m_nSelectGV == 0)
				cSingle->m_bGV = ChangeRGBtoGray((BYTE)cSingle->m_sColor.bRed, (BYTE)cSingle->m_sColor.bGreen, (BYTE)cSingle->m_sColor.bBlue);
			else
				cSingle->m_bGV = cSingle->m_nSelectGV;
			GetImageInfo()->SetValue((int)rpViewPoint.x, (int)rpViewPoint.y, (BYTE)cSingle->m_bGV);
		}

		if(1 < m_rpBeforePoint.GetDistance(rpViewPoint) && m_rpBeforePoint.x != 0 && m_rpBeforePoint.y != 0)
		{
			CRavidPoint<int> rpVector = m_rpBeforePoint.GetUnitVector(rpViewPoint);

			//while(true)
			//{
			//	CRavidPoint<int> rpAdd;
 		//		rpAdd.x = m_rpBeforePoint.x + rpVector.x;
			//	rpAdd.y = m_rpBeforePoint.y + rpVector.y;

			//	m_rpMap[rpAdd.x].push(rpAdd.y);

			//	m_rpBeforePoint = rpAdd;
			//	if(rpAdd.GetDistance(rpViewPoint) < 1 || (rpVector.x == 0 && rpVector.y == 0))
			//		break;
			//}
		}


		m_rpMap[rpViewPoint.x].push((int)rpViewPoint.y);
		m_rpBeforePoint = rpViewPoint;

		if(rpViewPoint.x < m_cpLButtonROILeftTop.x)
			m_cpLButtonROILeftTop.x = (int)rpViewPoint.x;
		if(rpViewPoint.y < m_cpLButtonROILeftTop.y)
			m_cpLButtonROILeftTop.y = (int)rpViewPoint.y;
		if(m_cpLButtonROIRightBottom.x < rpViewPoint.x)
			m_cpLButtonROIRightBottom.x = (int)rpViewPoint.x;
		if(m_cpLButtonROIRightBottom.y < rpViewPoint.y)
			m_cpLButtonROIRightBottom.y = (int)rpViewPoint.y;

		Invalidate();
	}
	else if(IsMButtonClicking() && IsUsePen())
	{
		CRavidPoint<int> rpTemp = point;
		rpTemp = ScreenCoordToImageCoord(rpTemp);
		CRavidPoint<double> rpViewPoint = rpTemp;

		if(m_rpBeforePoint.x == rpViewPoint.x && m_rpBeforePoint.y == rpViewPoint.y)
			return;



		m_vctMPoint.push_back(rpViewPoint);
		m_rpBeforePoint = rpViewPoint;
		m_rpMap[rpViewPoint.x].push(rpViewPoint.y);

	}
	else
		__super::OnMouseMove(nFlags, point);
}

void CUtilPen::OnLButtonDown(UINT nFlags, CPoint point)
{
	int tes = GetObjectID();
	CSingletonCheck* cSingle;
	cSingle = CSingletonCheck::GetInstance();
	if (cSingle->m_bSelectGV)
	{
		if (GetImageInfo() == NULL)
			return;

		CRavidPoint<int> rpTemp = point;
		rpTemp = ScreenCoordToImageCoord(rpTemp);
		CRavidPoint<double> rpViewPoint = rpTemp;
		float fPixel = GetImageInfo()->GetGVBilinear(rpViewPoint);

		cSingle->m_nSelectGV = (BYTE)fPixel;
		cSingle->m_bSelectGV = false;
	}
	else
	{
		if (IsUsePen())
		{
			if (GetImageInfo() == NULL)
				return;

			m_rpMap.clear();
			m_rpBeforePoint.x = 0;
			m_rpBeforePoint.y = 0;
			m_bLButtonClicking = true;

			CRavidPoint<int> rpTemp = point;
			rpTemp = ScreenCoordToImageCoord(rpTemp);
			CRavidPoint<double> rpViewPoint = rpTemp;
			double dblPixel = GetImageInfo()->GetGVBilinear(rpViewPoint);
			m_rpFirstPoint = rpViewPoint;
			m_rpBeforePoint = rpViewPoint;
			if (GetImageInfo()->IsColor())
			{
				GetImageInfo()->SetValue((int)rpViewPoint.x, (int)rpViewPoint.y, (BYTE)cSingle->m_sColor.bRed, (BYTE)cSingle->m_sColor.bGreen, (BYTE)cSingle->m_sColor.bBlue);
			}
			else
			{
				if (cSingle->m_nSelectGV == 0)
					cSingle->m_bGV = ChangeRGBtoGray((BYTE)cSingle->m_sColor.bRed, (BYTE)cSingle->m_sColor.bGreen, (BYTE)cSingle->m_sColor.bBlue);
				else
					cSingle->m_bGV = cSingle->m_nSelectGV;
				GetImageInfo()->SetValue((int)rpViewPoint.x, (int)rpViewPoint.y, (BYTE)cSingle->m_bGV);
			}
			Invalidate();
		}
		else
			__super::OnLButtonDown(nFlags, point);
	}
	
}

void CUtilPen::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (IsUsePen() && GetImageInfo())
	{
		CRavidPoint<double> rpVector = m_rpBeforePoint.GetUnitVector(m_rpFirstPoint);

		//while(true)
		//{
		//	CRavidPoint<double> rpAdd;
		//	rpAdd.x = m_rpBeforePoint.x + rpVector.x;
		//	rpAdd.y = m_rpBeforePoint.y + rpVector.y;

		//	m_rpMap[rpAdd.x].push(rpAdd.y);

		//	m_rpBeforePoint = rpAdd;
		//	if(rpAdd.GetDistance(m_rpFirstPoint) < 1 || (rpVector.x == 0 && rpVector.y == 0))
		//		break;
		//}

		m_bLButtonClicking = false;


	}
	else
	__super::OnLButtonUp(nFlags, point);
}


void CUtilPen::OnMButtonDown(UINT nFlags, CPoint point)
{
	if (IsUsePen())
	{
		m_bMButtonClicking = true;

		m_cpWheelStart = point;
	}
	else
		__super::OnMButtonDown(nFlags, point);

}
void CUtilPen::OnMButtonUp(UINT nFlags, CPoint point)
{
	if (IsUsePen() && GetImageInfo())
	{
		m_bMButtonClicking = false;

		PaintPolygon();
		m_rpMap.clear();

		CSingletonCheck* cSingle;
		cSingle = CSingletonCheck::GetInstance();

		m_cpWheelEnd = point;

		CRavidRect<int> rrROI;

		rrROI.left = m_cpWheelStart.x < m_cpWheelEnd.x ? m_cpWheelStart.x : m_cpWheelEnd.x;
		rrROI.top = m_cpWheelStart.y < m_cpWheelEnd.y ? m_cpWheelStart.y : m_cpWheelEnd.y;
		rrROI.right = m_cpWheelStart.x < m_cpWheelEnd.x ? m_cpWheelEnd.x : m_cpWheelStart.x;
		rrROI.bottom = m_cpWheelStart.y < m_cpWheelEnd.y ? m_cpWheelEnd.y : m_cpWheelStart.y;

		for(int i = rrROI.top; i < rrROI.bottom; i++)
		{
			for(int j = rrROI.left; j < rrROI.right; j++)
			{
				CRavidPoint<int> rpTest;
				rpTest.x = j;
				rpTest.y = i;

				rpTest = ScreenCoordToImageCoord(rpTest);

				if(cSingle->m_nSelectGV == 0)
					cSingle->m_bGV = ChangeRGBtoGray((BYTE)cSingle->m_sColor.bRed, (BYTE)cSingle->m_sColor.bGreen, (BYTE)cSingle->m_sColor.bBlue);
				else
					cSingle->m_bGV = cSingle->m_nSelectGV;

				if(0 < rpTest.x && 0 < rpTest.y && rpTest.x < GetImageInfo()->GetSizeX() && rpTest.y < GetImageInfo()->GetSizeY())
					GetImageInfo()->SetValue(rpTest.x, rpTest.y, (BYTE)cSingle->m_bGV);
			}
		}
		Invalidate();

	}
	else
		__super::OnMButtonUp(nFlags, point);
}


void CUtilPen::PaintPolygon()
{
	CRavidRect<int> rrROI;
	rrROI.left = m_cpLButtonROILeftTop.x - 10;
	rrROI.top = m_cpLButtonROILeftTop.y - 10;
	rrROI.right = m_cpLButtonROIRightBottom.x + 10;
	rrROI.bottom = m_cpLButtonROIRightBottom.y + 10;

	CSingletonCheck* cSingle;
	cSingle = CSingletonCheck::GetInstance();

	for(std::unordered_map<int, std::priority_queue<int, std::vector<int>, std::greater<int>>>::iterator it = m_rpMap.begin(); it != m_rpMap.end(); it++)
	{
		if(it->second.size() < 2)
			continue;

		int nFirst = it->second.top();
		it->second.pop();
		int nSecond = it->second.top();
		it->second.pop();

		while(true)
		{
			if(abs(nFirst - nSecond) <= 1 && it->second.size())
			{
				nFirst = nSecond;
				nSecond = it->second.top();
				it->second.pop();
			}
			else
				break;
		}

		for(nFirst; nFirst < nSecond; nFirst++)
		{
			GetImageInfo()->SetValue(it->first, nFirst, (BYTE)cSingle->m_bGV);

		}
	}

}


BOOL CUtilPen::PreTranslateMessage(MSG* pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

void CUtilPen::ChoiceColor()
{
	CSingletonCheck* cSingle;
	cSingle = CSingletonCheck::GetInstance();

	cSingle->m_nSelectGV = 0;

 	 int nYes = AfxMessageBox(_T("색 배합 할꺼?"), MB_YESNO);
  	if (nYes == 6)
  	{
		SelectDialogColor();
  	}
  	else
	{
		SelectImageColor();
	}
}


void CUtilPen::SelectDialogColor()
{
	CSingletonCheck* cSingle;
	cSingle = CSingletonCheck::GetInstance();

	int tes = GetObjectID();

	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);;
	if ((dlg.DoModal() == IDOK))
	{
		cSingle->m_sColor.bRed = GetRValue(dlg.GetColor());
		cSingle->m_sColor.bGreen = GetGValue(dlg.GetColor());
		cSingle->m_sColor.bBlue = GetBValue(dlg.GetColor());
	}
}

void CUtilPen::SelectImageColor()
{
	CSingletonCheck* cSingle;

	cSingle = CSingletonCheck::GetInstance();

	cSingle->m_bSelectGV = true;
}



void CUtilPen::UsePen(bool bUse)
{
	CSingletonCheck* cSingle;

	cSingle = CSingletonCheck::GetInstance();
	cSingle->SetUse(bUse);

}

bool CUtilPen::IsUsePen()
{
	CSingletonCheck* cSingle;

	cSingle = CSingletonCheck::GetInstance();
	return cSingle->IsUsePen();

}

BYTE CUtilPen::ChangeRGBtoGray(BYTE bR, BYTE bG, BYTE bB)
{
	//	RGB to YIQ Matrix 사용한 변환공식.
	return (BYTE)((0.2989 * bR) + (0.5870 * bG) + (0.1140 * bB));
}







bool CSingletonCheck::bFlag = false;
CSingletonCheck* CSingletonCheck::cSingleInstance = nullptr;

CSingletonCheck* CSingletonCheck::GetInstance() 
{
	if (!cSingleInstance)
	{
		cSingleInstance = new CSingletonCheck();
		bFlag = true;
	}
	return cSingleInstance;
}