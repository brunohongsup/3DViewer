#pragma once
#include "RavidFramework/RavidImageView.h"

#include <unordered_map>

class CHeightMap3D;

class CImageView3D : public Ravid::Framework::CRavidImageView
{
	RavidUseDynamicCreation();

protected: // serialization에서만 만들어집니다.
	CImageView3D();

protected:
	DECLARE_DYNCREATE(CImageView3D)

public:
	virtual ~CImageView3D();

	CRavidDoc* GetDocument() const;

#ifdef _DEBUG	
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.

public:
	DECLARE_MESSAGE_MAP()


protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

	CHeightMap3D* GetMpa(){return m_pMpaCls;}

	CPoint m_Startpoint;
	CPoint m_Endpoint;

	bool m_bDraw;
public:
	CHeightMap3D* m_pMpaCls;
};

