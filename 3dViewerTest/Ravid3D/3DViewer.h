#pragma once

#include "../RavidCore/PerformanceCounter.h"

class CRavid3D;
class CRavid3DData;
class AFX_EXT_CLASS C3DViewer : public CView
{
	DECLARE_DYNCREATE(C3DViewer)

protected:
	C3DViewer(void);
	virtual ~C3DViewer(void);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	virtual bool AddMap(_In_ CRavid3DData* pData);

	virtual bool Clear();

protected:
	static UINT RenderingThread(_In_ LPVOID pParam);

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual void OnInitialUpdate();

	virtual void OnDraw(CDC* pDC);

	afx_msg void OnDestroy();

	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	CRavid3D* m_pRenderer;

	HWND m_hWnd;
	HDC	m_hDC;
	HGLRC m_hRC;

	CWinThread* m_pLiveThread;

	bool m_bThreadLoop;

	CPoint m_ptBaseCoordinate;
	CPoint m_ptCurrentCoordinate;

	Ravid::Miscellaneous::CPerformanceCounter m_pc;
	bool m_bFPS;
};

