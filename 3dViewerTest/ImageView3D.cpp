#include "stdafx.h"
#include "ImageView3D.h"

#include "HeightMap3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CImageView3D, CRavidImageView)

CImageView3D::CImageView3D() : 
	m_bDraw(false)
{
	m_pMpaCls = new CHeightMap3D;

	m_bShowPixelValue = false;
}

CImageView3D::~CImageView3D()
{
	if (GetSafeHwnd())
		this->SetImageInfo(nullptr);

	if (m_pMpaCls)
		delete m_pMpaCls;
}

#ifdef _DEBUG
void CImageView3D::AssertValid() const
{
	CView::AssertValid();
}

void CImageView3D::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif

CRavidDoc* CImageView3D::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRavidDoc)));
	return (CRavidDoc*)m_pDocument;
}

BEGIN_MESSAGE_MAP(CImageView3D, CRavidImageView)
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CImageView3D::OnDraw(CDC * pDC)
{

	__super::OnDraw(pDC);
}

void CImageView3D::OnRButtonUp(UINT nFlags, CPoint point)
{
	if(nFlags == MK_SHIFT)
	{
		__super::OnRButtonUp(nFlags, point);
	}
	else
	{
		CMenu menu;
		menu.CreatePopupMenu();

		menu.AppendMenu(MF_STRING, (UINT)10000, _T("[View] Open"));
		menu.AppendMenu(MF_STRING, (UINT)10006, _T("[View] SetData"));
		menu.AppendMenu(MF_STRING, (UINT)10001, _T("[View] Convert"));
		menu.AppendMenu(MF_STRING, (UINT)10002, _T("[View] Height Map"));
		menu.AppendMenu(MF_STRING, (UINT)10003, _T("[View] Height Sudo"));
		menu.AppendMenu(MF_STRING, (UINT)10004, _T("[Set] Setting"));
		menu.AppendMenu(MF_STRING, (UINT)10005, _T("[Set] Clear"));

		CRavidPoint<int> pt = ScreenCoordToImageCoord(point.x, point.y);

		POINT ptBase = { point.x, point.y };
		ClientToScreen(&ptBase);

		UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
		int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, ptBase.x, ptBase.y, this);
		if (!nRetValue)
			return;

		switch (nRetValue)
		{
		case 10000:
			this->OnImageOpen();
		case 10006:
			m_pMpaCls->SetMap(GetImageInfo());
			break;
		case 10001:
		{
			if (GetSafeHwnd())
				this->SetImageInfo(nullptr);
			
			m_pMpaCls->Convert();

			if (GetSafeHwnd())
				this->SetImageInfo(m_pMpaCls->GetHeightMap());
		}
		break;
		case 10002:
		{
			this->SetImageInfo(m_pMpaCls->GetHeightMap(), false);
		}
		break;
		case 10003:
		{
			this->SetImageInfo(m_pMpaCls->GetColorSudoMap(), false);
		}
		break;
		case 10004:
		{
			CString strValue;

			CRavidInputBoxInfo ribi;
			ribi.AddInputDataInfo(_T("IsUsed?"), ERavidInputBoxFieldType_Combo, _T(""), false, _T("0;1;2;3;4;5;"));
			ribi.AddInputDataInfo(_T("Limit Invalid Value"));
			ribi.AddInputDataInfo(_T("Lower"));
			ribi.AddInputDataInfo(_T("Middle"));
			ribi.AddInputDataInfo(_T("Upper"));

			strValue.Format(_T("%d"), m_pMpaCls->m_nUsedSetting);
			ribi.SetInputData(0, strValue);
			strValue.Format(_T("%.3f"), m_pMpaCls->m_fInvalidValue);
			ribi.SetInputData(1, strValue);
			strValue.Format(_T("%.3f"), m_pMpaCls->m_fLow);
			ribi.SetInputData(2, strValue);
			strValue.Format(_T("%.3f"), m_pMpaCls->m_fMid);
			ribi.SetInputData(3, strValue);
			strValue.Format(_T("%.3f"), m_pMpaCls->m_fUp);
			ribi.SetInputData(4, strValue);

			if(CUIManager::RunRavidInputBox(&ribi))
			{
				m_pMpaCls->m_nUsedSetting = _ttoi(ribi.GetInputData(0));
				m_pMpaCls->m_fInvalidValue = (float)_ttof(ribi.GetInputData(1));
				m_pMpaCls->m_fLow = (float)_ttof(ribi.GetInputData(2));
				m_pMpaCls->m_fMid = (float)_ttof(ribi.GetInputData(3));
				m_pMpaCls->m_fUp = (float)_ttof(ribi.GetInputData(4));
			}
			
			if(m_pMpaCls->m_nUsedSetting != 0)
			{
				CColorDialog cd;// (m_dwLowerColor, CC_FULLOPEN);
				cd.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
				cd.m_cc.rgbResult = m_pMpaCls->m_dwLowColor;
				if (cd.DoModal() == IDOK)
					m_pMpaCls->m_dwLowColor = cd.GetColor();

				cd.m_cc.rgbResult = m_pMpaCls->m_dwMidColor;
				if (cd.DoModal() == IDOK)
					m_pMpaCls->m_dwMidColor = cd.GetColor();

				cd.m_cc.rgbResult = m_pMpaCls->m_dwUpColor;
				if (cd.DoModal() == IDOK)
					m_pMpaCls->m_dwUpColor = cd.GetColor();
			}			
		}
		break;
		case 10005:
		{
			if (GetSafeHwnd())
				this->SetImageInfo(nullptr);

			m_pMpaCls->Clear();
		}
		break;
		default:
			break;
		}
	}
}

void CImageView3D::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRavidPoint<int> pt = ScreenCoordToImageCoord(point.x, point.y);
	m_Startpoint = pt;
	m_Endpoint = pt;
	Invalidate(false);
}

void CImageView3D::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRavidPoint<int> pt = ScreenCoordToImageCoord(point.x, point.y);
	m_Endpoint = pt;
	Invalidate(false);
}


void CImageView3D::OnMouseMove(UINT nFlags, CPoint point)
{
	CRavidPoint<int> pt = ScreenCoordToImageCoord(point.x, point.y);
	m_Endpoint = pt;
	Invalidate(false);
		
	if (nFlags == MK_SHIFT)
		m_bDraw = true;

	__super::OnMouseMove(nFlags, point);
}


void CImageView3D::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CRavidImageView::OnPaint()을(를) 호출하지 마십시오.

	CRavidImageView::OnDraw(&dc);

	if (m_pMpaCls->GetHeightMap())
	{
		do 
		{
			if (m_bDraw)
			{
				m_bDraw = false;

				GetLayer(0)->Clear();

				for (int y = -5; y <= 5; ++y)
				{
					int posY = m_Endpoint.y + y;

					for (int x = -5; x <= 5; ++x)
					{
						int posX = m_Endpoint.x + x;

						float fData = m_pMpaCls->GetHeight(posX, posY);

						CString str;
						str.Format(_T("%.3f"), fData);

						GetLayer(0)->DrawTextW(CRavidPoint<double>(posX, posY) + 0.5, str, BLACK, LIME);
					}
				}

// 				BYTE** ppSrc = m_pMpaCls->GetHeightMap()->GetYOffsetTable();
// 				size_t* pSrc = m_pMpaCls->GetHeightMap()->GetXOffsetTable();
// 				for (int y = -5; y <= 5; ++y)
// 				{
// 					int posY = m_Endpoint.y + y;
// 					if (posY < 0 || posY >= m_pMpaCls->GetHeightMap()->GetSizeY())
// 						continue;
// 
// 					for (int x = -5; x <= 5; ++x)
// 					{
// 						int posX = m_Endpoint.x + x;
// 						if (posX < 0 || posX >= m_pMpaCls->GetHeightMap()->GetSizeX())
// 							continue;
// 
// 						float fData = *(float*)(ppSrc[posY] + pSrc[posX]);
// 
// 						CString str;
// 						str.Format(_T("%.3f"), fData);
// 
// 						GetLayer(0)->DrawTextW(CRavidPoint<double>(posX, posY) + 0.5, str, BLACK, LIME);
// 					}
// 				}
			}
		}
		while(false);
	}
}
