#pragma once
using namespace Ravid::Framework;
using namespace Ravid;

#include <unordered_map>

class CUtilPen : public CRavidImageView
{
public:

	//211005version

	//ImageView에 상속받아서 사용.


	//사용법
	//1. Use Pen 함수로 펜사용 선택
	//2. ChioceColor로 RGB선택할지, 이미지에서 GV얻어올지 선택하고 그리면 됨.
	//3. LButton Down up / MButton Down up / MouseMove 이벤트를 가져오기위해 아래 함수 ImageView.Cpp에 넣어줄것.


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


	//LButton 다운으로 대충 그리고 M버튼 클릭 시, 도형내부 채움.
	//M버튼으로 드래그 할 시, 드래그영역 사각형 채움.



	void UsePen(bool bUse); // 펜사용 할지말지 선택
	void ChoiceColor(); // 색선택 할지 말지 선택. Yes -> RGB색상창 / No -> 스포이드













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


