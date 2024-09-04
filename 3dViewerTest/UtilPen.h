#pragma once
using namespace Ravid::Framework;
using namespace Ravid;

#include <unordered_map>

class CUtilPen : public CRavidImageView
{
public:

	//211005version

	//ImageView�� ��ӹ޾Ƽ� ���.


	//����
	//1. Use Pen �Լ��� ���� ����
	//2. ChioceColor�� RGB��������, �̹������� GV������ �����ϰ� �׸��� ��.
	//3. LButton Down up / MButton Down up / MouseMove �̺�Ʈ�� ������������ �Ʒ� �Լ� ImageView.Cpp�� �־��ٰ�.


	//void CImageViewMain::OnLButtonDown(UINT nFlags, CPoint point)
	//{
	//		__super::OnLButtonDown(nFlags, point);
	//}
	//void CImageViewMain::OnLButtonUp(UINT nFlags, CPoint point)
	//{
	//		__super::OnLButtonUp(nFlags, point);
	//}
	//void CImageViewMain::OnMButtonDown(UINT nFlags, CPoint point)
	//{
	//	__super::OnMButtonDown(nFlags, point);
	//}
	//void CImageViewMain::OnMButtonUp(UINT nFlags, CPoint point)
	//{
	//	__super::OnMButtonUp(nFlags, point);
	//}
	//void CImageViewMain::OnMouseMove(UINT nFlags, CPoint point)
	//{
	//	__super::OnMouseMove(nFlags, point);
	//}


	//LButton �ٿ����� ���� �׸��� M��ư Ŭ�� ��, �������� ä��.
	//M��ư���� �巡�� �� ��, �巡�׿��� �簢�� ä��.



	void UsePen(bool bUse); // ���� �������� ����
	void ChoiceColor(); // ������ ���� ���� ����. Yes -> RGB����â / No -> �����̵�













	CUtilPen();
	~CUtilPen();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);

	BOOL PreTranslateMessage(MSG* pMsg);
	void OnDraw(CDC* pDC);

private:
	

	bool IsUsePen();
	bool m_bUsePen;

	CPoint m_cpWheelStart;
	CPoint m_cpWheelEnd;
	bool IsMButtonClicking(){return m_bMButtonClicking;}
	void PaintPolygon();
	std::vector<CRavidPoint<double>> m_vctMPoint;
	CRavidPoint<int> m_rpBeforePoint;
	bool m_bMButtonClicking;

	bool IsLButtonClicking(){return m_bLButtonClicking;}
	bool m_bLButtonClicking;
	CPoint m_cpLButtonROILeftTop;
	CPoint m_cpLButtonROIRightBottom;
	CRavidPoint<double> m_rpFirstPoint;

	std::unordered_map<int, std::priority_queue<int, std::vector<int>, std::greater<int>>> m_rpMap;

	void SelectDialogColor();
	void SelectImageColor();

	BYTE ChangeRGBtoGray(BYTE bR, BYTE bG, BYTE bB);
};


class CSingletonCheck 
{
public:
	struct sColor
	{
		BYTE bRed;
		BYTE bGreen;
		BYTE bBlue;
	};
	sColor m_sColor;

	BYTE m_bGV;

	bool m_bSelectGV;
	BYTE m_nSelectGV;

	CSingletonCheck() { bUse = false; }

	static bool bFlag;
	static CSingletonCheck* cSingleInstance;

	bool bUse;
public:

	static CSingletonCheck* GetInstance();

	virtual ~CSingletonCheck() {
		bFlag = false;
	};

	void SetUse(bool bUsePen) { bUse = bUsePen; }
	bool IsUsePen() { return bUse; }

};


