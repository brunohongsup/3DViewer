
// RavidChildFrameBase.h : CRavidChildFrameBase 클래스의 인터페이스
//

#pragma once

#include "RavidObjectEx.h"

#include "../RavidCore/RavidRect.h"

namespace Ravid
{
	namespace Framework
	{
		class CRavidToolBar;
		class CRavidImageViewScrollBar;
		class CRavidSlider;

		class AFX_EXT_CLASS CRavidChildFrameBase : public CMDIChildWndEx, public CRavidObjectEx
		{
			RavidUseDynamicCreation();

			DECLARE_DYNCREATE(CRavidChildFrameBase)
		public:
			CRavidChildFrameBase();
			CRavidChildFrameBase(const CRavidChildFrameBase& rcfb) { }

		// 특성입니다.
		public:

		// 작업입니다.
		public:

		// 재정의입니다.
			virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

		// 구현입니다.
		public:
			virtual ~CRavidChildFrameBase();
#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
		protected:
			DECLARE_MESSAGE_MAP()
		public:
			afx_msg void OnPaint();
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
			virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
			virtual void OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt);
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnChildActivate();
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
			afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
			afx_msg void OnNcMouseHover(UINT nFlags, CPoint point);
			afx_msg void OnMouseLeave();
			afx_msg void OnMouseHover(UINT nFlags, CPoint point);
			afx_msg void OnSetFocus(CWnd* pOldWnd);
			afx_msg void OnBnClickedMaximize();
			afx_msg void OnBnClickedRestore();
			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			afx_msg void OnTimer(UINT_PTR nIDEvent);

			afx_msg LRESULT OnUpdateToolbarStatus(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnMaximize(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnRestore(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnSetTitle(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnEnableScrollBar(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnUpdateScrollBar(WPARAM wParam, LPARAM lParam);

			virtual bool InitialzeToolBar();

			bool GetAllChildFrame(_Out_ std::vector<CRavidChildFrameBase*>* pVctChildFrame);

			bool IsNeighborLine(_In_ CPoint ptStart1, _In_ CPoint ptEnd1, _In_ CPoint ptStart2, _In_ CPoint ptEnd2);

			bool FindNeighborChildFrame(_In_ SRavidChildFrameRect* pCFRTarget, _In_ std::vector<SRavidChildFrameRect>* pVctSCFR, _Out_ std::vector<SRavidFrameNeighborChildFrame>* pVctSNCF);

			virtual CRavidRect<int> GetRavidMainFrameClientRect();

			CRavidRect<int> GetFrameRect();
			bool ScreenToFrame(_Inout_ CRect& cr);
			bool ScreenToFrame(_Inout_ CPoint& pt);

			void SetFramePercentage(_In_ CRavidRect<double> drcPercentage);
			CRavidRect<double> GetFramePercentage();

			void SetFrameCoordinate(_In_ CRavidRect<int> drcCoordinate);
			CRavidRect<int> GetFrameCoordinate();

			void LockRelative();
			void UnlockRelative();
			bool IsLockRelative();

			virtual bool Maximize();
			virtual bool Restore();

			bool IsMaximized();

			virtual void SetFrameSizeChangeMode(_In_ ERavidFrameSizeChangeMode eFSCM);
			ERavidFrameSizeChangeMode GetFrameSizeChangeMode();

			ERavidFrameSizeChangeMode GetFrameSizeChangeModeFromPos(CPoint pt);

			void SetMinimumFrameSize(_In_ int nX, _In_ int nY);
			CSize GetMinimumFrameSize();

			bool UpdateToolBar();

			virtual bool UpdateToolbarStatus();

			bool EnableToolBar(_In_ bool bEnable);
			bool IsEnableToolBar();

			bool SetTitle(_In_ CString strTitle);
			CString GetTitle();

			int GetToolBarHeight();

			virtual bool ChangeFramePosition(_In_ CRavidRect<int> rrOldPosition, _In_ CRavidRect<int> rrNewPosition, _Out_ std::vector<CRavidRect<int> >* pVctResultPosition);

			bool SetColor(_In_ ERavidFrameColorType eRFCT, _In_ COLORREF clr);
			COLORREF GetColor(_In_ ERavidFrameColorType eRFCT);

			bool SetToolBarColor(_In_ ERavidToolBarColorType eRTBCT, _In_ COLORREF clr);
			COLORREF GetToolBarColor(_In_ ERavidToolBarColorType eRTBCT);

			bool SetToolBarBottomEdgeDraw(_In_ bool bDraw);
			bool GetToolBarBottomEdgeDraw();

			bool SetToolBarTitleMinimumDisplayWidth(_In_ long nWidth);
			long GetToolBarTitleMinimumDisplayWidth();

			bool SetToolBarTitleStartOffsetX(_In_ long nOffsetX);
			long GetToolBarTitleStartOffsetX();

			void __fastcall SetObjectID(_In_ long nObjectID);

			bool EnableScrollBar(_In_ bool bEnable);
			bool IsScrollBarEnable();

			virtual bool UpdateScrollBar();

			CRavidImageViewScrollBar* GetVScrollBar();
			CRavidImageViewScrollBar* GetHScrollBar();

			void SaveConfigurationSettingTitle(_In_ CString strTitle);
			CString GetConfigurationSettingTitle();

			void SaveConfigurationSettingObjectID(_In_ int nObjectID);
			int GetConfigurationSettingObjectID();

			virtual bool VisibleToolBarButton(_In_ ERavidToolBarEventChildFrameBase eEvent, _In_ bool bVisible);

		protected:
			ERavidFrameSizeChangeMode m_eFrameSizeChangeMode = ERavidFrameSizeChangeMode_None;
			CPoint m_ptFrameSizeChangeDefault = CPoint(0, 0);
			CPoint m_ptFrameSizeChangeDiff = CPoint(0, 0);

			CRect m_crDefaultFrame = CRect(0, 0, 0, 0);
			CRavidRect<double> m_drcFramePercentage;
			CRavidRect<int> m_drcFrameCoordinate;
			CSize m_szMinimumFrame = CSize(50, 50);
			bool m_bLockRelative = true;

			CRavidToolBar* m_pWndToolBar = nullptr;

			bool m_bIsMaximized = false;
			CRavidRect<double> m_drcRestoreFramePercentage;
			CRavidRect<double> m_drcRestoreFrameCoordinate;

			CRavidSlider* m_pRS = nullptr;

			CRavidImageViewScrollBar* m_pVScrollBar = nullptr;
			CRavidImageViewScrollBar* m_pHScrollBar = nullptr;

			bool m_bShowScrollBar = false;

			CToolTipCtrl m_ctrlToolTip;

			LPCTSTR m_lpszSelectedCursor = nullptr;

			COLORREF m_clrFrame[ERavidFrameColorType_Count] = { 0, };

			int m_nMagnetSize = 5;

			CString m_strConfigurationTitle = _T("");
			int m_nConfigurationObjectID = 0;

			bool m_bLBtnDown = false;
			bool m_bMouseMove = false;

			HCURSOR m_hCursor;

			std::vector<std::pair<CMFCToolBarButton*, bool>> m_vctBtn;
		};
	}
}
