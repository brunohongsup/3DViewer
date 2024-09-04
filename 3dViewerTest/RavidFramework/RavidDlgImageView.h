#pragma once

#include "RavidDialogBase.h"
#include "RavidImageView.h"

// CRavidDlgImageView 대화 상자

namespace Ravid
{
	namespace Framework
	{
		class CRavidToolBar;
		class CRavidImageViewScrollBar;
		class AFX_EXT_CLASS CRavidDlgImageView : public CRavidDialogBase
		{
			RavidUseDynamicCreation();

			DECLARE_DYNCREATE(CRavidDlgImageView)

			enum ERavidDlgImageViewPopupMenu
			{
				ERavidDlgImageViewPopupMenu_Distance,
				ERavidDlgImageViewPopupMenu_DistanceCircle,
				ERavidDlgImageViewPopupMenu_DistanceCircumcenter,
				ERavidDlgImageViewPopupMenu_DistanceParallel,
				ERavidDlgImageViewPopupMenu_Displacement,
				ERavidDlgImageViewPopupMenu_AngleMeasurement,
				ERavidDlgImageViewPopupMenu_AngleAzimuth,
				ERavidDlgImageViewPopupMenu_AngleGradient,
				ERavidDlgImageViewPopupMenu_Area,
				ERavidDlgImageViewPopupMenu_Count,
			};

		public:
			CRavidDlgImageView(CWnd* pParent = nullptr);   // 표준 생성자입니다.
			virtual ~CRavidDlgImageView();

		// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_RAVID_DLG_VIEW };
#endif

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

		public:
			virtual BOOL Create(_In_ DWORD dwWindowStyle = WS_POPUP, _In_opt_ CWnd* pParent = nullptr, _In_opt_ CRuntimeClass* pView = RUNTIME_CLASS(CRavidImageView));

			virtual bool SetTitle(_In_ CString strTitle);
			virtual CString GetTitle();

			virtual bool SetObjectID(_In_ int nObjectID);
			virtual int GetObjectID();

			virtual CRavidImageView* GetActiveView();

			virtual int GetToolBarHeight();
			virtual bool IsEnableToolBar();

			virtual bool EnableScrollBar(_In_ bool bEnable);
			virtual bool IsScrollBarEnable();

			virtual bool UpdateScrollBar();

			virtual bool EnablePopupMenu(_In_ bool bEnable);

			virtual bool SetEdgeThickness(_In_ int nThicknessX, _In_ int nThicknessY);
			virtual bool GetEdgeThickness(_Out_ int* pThicknessX, _Out_ int* pThicknessY);

			virtual void CloseDialogView();

			virtual bool InitialzeToolBar();

			virtual bool IsMaximized();

			virtual bool Maximize();
			virtual bool Restore();

			virtual DWORD GetWindowStyle();

			virtual CRavidImageViewScrollBar* GetVScrollBar();
			virtual CRavidImageViewScrollBar* GetHScrollBar();

			virtual bool UpdateToolbarStatus();

			virtual bool UpdateCursorModeButton(_In_ ERavidImageViewCursorMode eSelectedMode);

			virtual bool VisibleToolBarButton(_In_ ERavidToolBarEventDialogView eEvent, _In_ bool bVisible);

			virtual ERavidFrameSizeChangeMode GetFrameSizeChangeModeFromPos(_In_ CPoint pt);

			DECLARE_MESSAGE_MAP()
		public:
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

			virtual BOOL OnInitDialog();

			virtual BOOL PreTranslateMessage(MSG* pMsg);

			afx_msg void OnClose();

			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

			afx_msg void OnSize(UINT nType, int cx, int cy);

			afx_msg BOOL OnEraseBkgnd(CDC* pDC);

			afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

			afx_msg void OnBnClickedImageOpen();
			afx_msg void OnBnClickedImageSave();
			afx_msg void OnBnClickedImageClose();
			afx_msg void OnBnClickedInspect();
			afx_msg void OnBnClickedTeaching();
			afx_msg void OnBnClickedEditTeaching();
			afx_msg void OnBnClickedArrow();
			afx_msg void OnBnClickedZoomIn();
			afx_msg void OnBnClickedZoomOut();
			afx_msg void OnBnClickedZoomAll();
			afx_msg void OnBnClickedMove();
			afx_msg void OnBnClickedMeasure();
			afx_msg void OnBnClickedLayerOnOff();
			afx_msg void OnBnClickedGridOn();
			afx_msg void OnBnClickedGridOff();
			afx_msg void OnBnClickedGVColorOn();
			afx_msg void OnBnClickedGVColorOff();
			afx_msg void OnBnClickedGVOn();
			afx_msg void OnBnClickedGVOff();
			afx_msg void OnBnClickedAccuracySetting();
			afx_msg void OnBnClickedStatusBarOn();
			afx_msg void OnBnClickedStatusBarOff();
			afx_msg void OnBnClickedScrollBarOn();
			afx_msg void OnBnClickedScrollBarOff();
			afx_msg void OnBnClickedMaximize();
			afx_msg void OnBnClickedRestore();
			afx_msg void OnBnClickedClose();

			afx_msg LRESULT OnUpdateToolbarStatus(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnMaximize(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnRestore(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnSetTitle(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnEnableScrollBar(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnUpdateCursorModeButton(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnUpdateScrollBar(WPARAM wParam, LPARAM lParam);

		private:
			CRuntimeClass* m_pView = nullptr;
			CCreateContext m_context;

			CRavidImageView* m_pActiveView = nullptr;

			CRavidToolBar* m_pWndToolBar = nullptr;

			CToolTipCtrl m_ctrlToolTip;

			CSize m_sizeToolBar;
			CRect m_crOffset;

			CBitmap m_bmMenuIcons[ERavidDlgImageViewPopupMenu_Count];

			std::vector<std::pair<CMFCToolBarButton*, bool>> m_vctBtn;

			CRavidImageViewScrollBar* m_pVScrollBar = nullptr;
			CRavidImageViewScrollBar* m_pHScrollBar = nullptr;

			bool m_bShowScrollBar = false;

			int m_nThicknessX = 0;
			int m_nThicknessY = 0;

			bool m_bMaximize = false;

			DWORD m_dwWindowStyle = WS_POPUP;
		};
	}
}
