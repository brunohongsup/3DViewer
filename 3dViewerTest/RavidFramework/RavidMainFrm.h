
// RavidMainFrm.h : CRavidMainFrame 클래스의 인터페이스
//

#pragma once

#include "RavidObjectEx.h"

#include <vector>

namespace Ravid
{
	namespace Miscellaneous
	{
		class CPerformanceCounter;
	}

	namespace Framework
	{
		class CRavidMenu;
		class CRavidToolBar;
		class CRavidChildFrameBase;
		class CRavidChildFrameLogViewer;
		class CRavidDlgModelManager;
		class CRavidDlgAuthorityManager;
		class CRavidChildFrameImageProcessingTool;
		class CRavidDlgScreenSaver;
		class CRavidDlgDeviceConfigurationTool;
		class CRavidDlgViewConfigurationTool;
		class CRavidViewConfigurationToolSlider;
		class CRavidDlgFrameworkConfigurationTool;
		class CRavidDlgConfigurationNew;

		class AFX_EXT_CLASS CRavidMainFrame : public CMDIFrameWndEx, public CRavidObjectEx
		{
			RavidUseDynamicCreation();

			DECLARE_DYNAMIC(CRavidMainFrame)

			enum ERavidMainFrameImage
			{
				ERavidMainFrameConfigurationIcon_Save = 0,
				ERavidMainFrameConfigurationIcon_SaveAs,
				ERavidMainFrameConfigurationIcon_New,
				ERavidMainFrameConfigurationIcon_Delete,
				ERavidMainFrameConfigurationIcon_Rename,
				ERavidMainFrameIcon_Count,
			};

		public:
			CRavidMainFrame();
			CRavidMainFrame(const CRavidMainFrame& rmf) { }

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			static LRESULT FAR PASCAL MDIWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

			// 특성입니다.
		public:

			// 작업입니다.
		public:

			// 재정의입니다.
		public:
			virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

			// 구현입니다.
		public:
			virtual ~CRavidMainFrame();
#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
#endif


	// 생성된 메시지 맵 함수
		protected:
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
			DECLARE_MESSAGE_MAP()
		public:
			afx_msg void OnSize(UINT nType, int cx, int cy);

			bool GetAllChildFrame(_Out_ std::vector<CRavidChildFrameBase*>* pVctChildFrame);

		public:
			virtual BOOL LoadFrameForToolBar(_In_ UINT nIDResource, _In_opt_ UINT ulToolBarResourceID = 0, _In_opt_ UINT ulDisabledToolBarResourceID = 0, _In_opt_ DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, _In_opt_ CWnd* pParentWnd = NULL, _In_opt_ CCreateContext* pContext = NULL);

			afx_msg void OnPaint();
			virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
			afx_msg void OnNcPaint();
			afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
			afx_msg BOOL OnNcActivate(BOOL bActive);
			afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
			afx_msg void OnDestroy();
			afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
			afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
			afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			afx_msg LRESULT OnMainFrameMessage(WPARAM wParam, LPARAM lParam);
			afx_msg LRESULT OnLogMessageForDev(WPARAM wParam, LPARAM lParam);
			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
			bool AdjustWindowRegion();

			CRavidChildFrameLogViewer* GetLogViewerFrame();
			CRavidChildFrameImageProcessingTool* GetImageProcessingToolFrame();

			CRavidDlgModelManager* GetDlgModelManager();

			void SetBorderThickness(_In_ int nL, _In_ int nT, _In_ int nR, _In_ int nB);

			bool IsEnableToolBar();

			int GetToolBarHeight();

			CRect GetFrameRect();
			bool FrameToScreen(_Inout_ CRect& cr);
			bool FrameToScreen(_Inout_ CRavidRect<int>& rr);

			bool UpdateToolbarStatus(_In_opt_ bool bUpdateChildFrameToolBar = true);

			virtual void VisibleLogViewer();
			virtual void InvisibleLogViewer();
			virtual bool IsVisibleLogViewer();

			virtual void VisibleImageProcessingTool();
			virtual void InvisibleImageProcessingTool();
			virtual bool IsVisibleImageProcessingTool();
			virtual bool UpdateImageProcessingToolSavedStatus();

			virtual void OpenModelManager();

			virtual void OpenAuthorityManager();

			virtual void SetScreenSaver();

			virtual void OpenDeviceConfigurationTool();

			virtual void OpenViewConfigurationTool();

			virtual void OpenFrameworkConfigurationTool();

			virtual bool LogIn();
			virtual bool LogOut();

			bool SetToolBarColor(_In_ ERavidToolBarColorType eRTBCT, _In_ COLORREF clr);
			COLORREF GetToolBarColor(_In_ ERavidToolBarColorType eRTBCT);

			bool SetToolBarBottomEdgeDraw(_In_ bool bDraw);
			bool GetToolBarBottomEdgeDraw();

			bool SetToolBarTitleMinimumDisplayWidth(_In_ long nWidth);
			long GetToolBarTitleMinimumDisplayWidth();

			bool SetToolBarTitleStartOffsetX(_In_ long nOffsetX);
			long GetToolBarTitleStartOffsetX();

			bool UseScreenSaver(_In_ bool bUse);
			bool IsUsedScreenSaver();

			bool SetScreenSaverTitle(_In_ CString strTitle);
			CString GetScreenSaverTitle();

			bool SetScreenSaverWaitingTime(_In_ DWORD dwTime);
			DWORD GetScreenSaverWaitingTime();

			bool UseScreenSaverLogIn(_In_ bool bUse);
			bool IsUsedScreenSaverLogIn();

			bool ShowToolBar(_In_ bool bShow);
			bool IsShowToolBar();

			bool RunRavidNotification(_In_ CString strMessage, _In_opt_ ERavidNotificationType eNotification, _In_opt_ ERavidModalType eModalType);

			virtual void OpenConfigurationNew();
			virtual void CloseConfigurationNew();

			bool VisibleToolBarEventBtnMenu(ERavidToolBarEventMainFrame eToolBarEventMainFrameMenu, bool bVisible = true);
			bool IsVisibleToolBarEventBtnMenu(ERavidToolBarEventMainFrame eToolBarEventMainFrameMenu);

			CRavidToolBar* GetToolBar();

			void UseBaseToolBarEvent(_In_ bool bUse);
			bool IsUsedBaseToolBarEvent();

			void EnableShowTitleBar(bool bEnable);
			bool IsEnabledShowTitleBar();

			void EnableShowTitleBarButton(ERavidCaptionButtonKind eKind, bool bEnable);
			bool IsEnabledShowTitleBarButton(ERavidCaptionButtonKind eKind);

			void EnableMoveWindowWithTitleBar(bool bEnable);
			bool IsEnabledMoveWindowWithTitleBar();

		private:

			bool DoIconChange();
			bool AdjustIconImagePointers(_In_ LPICONIMAGE lpImage);
			LPSTR FindDIBBits(_In_ LPSTR lpbi);
			WORD PaletteSize(_In_ LPSTR lpbi);
			DWORD BytesPerLine(_In_ LPBITMAPINFOHEADER lpBMIH);
			WORD DIBNumColors(_In_ LPSTR lpbi);
			DWORD WidthBytes(_In_ DWORD dwBits);
			DWORD WriteIconToICOFile(_In_ LPICONRESOURCE lpIR, _In_ LPCTSTR szFileName);
			DWORD WriteICOHeader(_In_ HANDLE hFile, _In_ UINT nNumEntries);
			DWORD CalculateImageOffset(_In_ LPICONRESOURCE lpIR, _In_ UINT nIndex);

			bool InitializeScreenSaver();

			CPoint GetClientCursorPos();

			CRect GetIconRect();
			virtual bool RunIconPopupMenu();

			static LONG_PTR m_lpfnMDIOldWndProc;

			CRavidToolBar* m_pWndToolBar = nullptr;

			CRect m_crPreFrame = CRect(0, 0, 0, 0);

			CRect m_crBorderThickness = CRect(0, 0, 0, 0);

			SRavidCaptionButtonInfo m_sRavidCaptionButtonInfos[ERavidCaptionButtonKind_Count];

			CRavidMenu* m_pMenu = nullptr;

			CRavidDlgModelManager* m_pDlgModelManger = nullptr;

			CRavidDlgAuthorityManager* m_pDlgAuthorityManager = nullptr;

			CRavidDlgScreenSaver* m_pDlgScreenSaver = nullptr;

			CRavidDlgDeviceConfigurationTool* m_pDlgDeviceConfigurationTool = nullptr;

			CRavidDlgViewConfigurationTool* m_pDlgViewConfigurationTool = nullptr;
			CRavidViewConfigurationToolSlider* m_pViewConfigurationToolSlider = nullptr;

			CRavidDlgFrameworkConfigurationTool* m_pDlgFrameworkConfigurationTool = nullptr;

			bool m_bScreenSaverInitializeDelay = true;
			Miscellaneous::CPerformanceCounter* m_pPCScreenSaverInitializeDelay = nullptr;

			bool m_bUsedScreenSaver = false;
			CString m_strScreenSaverTitle = _T("");
			DWORD m_dwScreenSaverWaitingTime = 30000;
			bool m_bUsedScreenSaverLogIn = false;

			UINT m_ulToolBarResourceID = 0;
			UINT m_ulDisabledToolBarResourceID = 0;

			bool m_bIsUsedUserToolBar = false;

			CFont m_fontTitle;

			HICON m_hIcon = 0;

			CBitmap m_bmpBackground;
			CSize m_szBackground = CSize(0, 0);

			CCriticalSection m_csNotification;
			std::vector<std::pair<CString, ERavidNotificationType> > m_vctNotification;

			CBitmap m_bmMainFrmIcons[ERavidMainFrameIcon_Count];

			CRavidDlgConfigurationNew* m_pDlgConfigurationNew = nullptr;
			CRavidViewConfigurationToolSlider* m_pConfigurationNewSlider = nullptr;

			bool m_bModifyVisibleToolBarEventBtn[ERavidToolBarEventMainFrame_Count] = { false, };

			bool m_bIsUsedBaseToolBarEvent = false;

			bool m_bIsEnabledShowTitleBar = true;
			bool m_bIsEnabledMoveWindowWithTitleBar = true;

		public:
			afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
			afx_msg void OnClose();
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
			afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
			afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
			afx_msg BOOL OnEraseBkgnd(CDC* pDC);
			afx_msg BOOL OnEraseMDIClientBackground(CDC* pDC);
			afx_msg LRESULT OnNcHitTest(CPoint point);
};
	}
}
