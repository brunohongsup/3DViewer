
// RavidToolBar.h : RavidToolBar 클래스의 인터페이스
//

#pragma once

#include <set>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidToolBar : public CMFCToolBar
		{
		public:
			RavidPreventCopySelf(CRavidToolBar);

			CRavidToolBar();
			virtual ~CRavidToolBar();

			void SetObjectID(_In_ long nObjectID);
			long GetObjectID();

			virtual BOOL LoadToolBar(UINT uiResID, UINT uiColdResID = 0, UINT uiMenuResID = 0, BOOL bLocked = FALSE, UINT uiDisabledResID = 0, UINT uiMenuDisabledResID = 0, UINT uiHotResID = 0);

			virtual void OnFillBackground(CDC* /*pDC*/);

			virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

			virtual BOOL LoadBitmapEx(CMFCToolBarInfo& params, BOOL bLocked = FALSE);

			void UpdateButton(int nIndex);

			virtual BOOL PreTranslateMessage(MSG* pMsg);

			bool Initialize();

			void SetButtonAlign(_In_ ERavidToolBarButtonAlign eAlign);
			ERavidToolBarButtonAlign GetButtonAlign();

			bool SetTitle(_In_ CString strTitle);
			CString GetTitle();

			void SetTitleMinimumDisplayWidth(_In_ long nWidth);
			long GetTitleMinimumDisplayWidth();

			void SetTitleStartOffsetX(_In_ long nOffsetX);
			long GetTitleStartOffsetX();

			long GetButtonStartX();

			bool UpdateToolBar();

			bool AddIndexWhichHadPopupMenu(_In_ long nIndex);
			bool RemoveIndexWhichHadPopupMenu(_In_ long nIndex);

			bool DoesIndexHavePopupMenu(_In_ long nIndex);

			void LockAutoPopup();
			void UnlockAutoPopup();
			bool IsLockAutoPopup();

			void SetExceptionTopThickness(_In_ long nExceptionTopThickness);
			long GetExceptionTopThickness();

			bool SetColor(_In_ ERavidToolBarColorType eRTBCT, _In_ COLORREF clr);
			COLORREF GetColor(_In_ ERavidToolBarColorType eRTBCT);

			bool SetDisabledButtonColor(_In_ COLORREF clr);

			bool SetBottomEdgeDraw(_In_ bool bDraw);
			bool GetBottomEdgeDraw();

			bool AddIndexAlwaysOn(_In_ long nIndex);
			bool RemoveIndexAlwaysOn(_In_ long nIndex);

			CString GetToolBarTitle(UINT uiToolBarID);

		protected:
			virtual BOOL DrawButton(CDC* pDC, CMFCToolBarButton* pButton, CMFCToolBarImages* pImages, BOOL bHighlighted, BOOL bDrawDisabledImages);

			virtual void AdjustLocations();

			bool OpenCustomizePopupMenu();

			bool IsAvailableCustomizePopupMenu();
			bool IsAvailablePopupMenu();

			bool DestroyCustomizePopupMenu();
			bool DestoryPopupMenu();

			bool IsAlreadyCustomizePopupMenu();
			bool IsAlreadyPopupMenu(int nNumber);

			bool ReadyOpenPopupMenuFromPoint(_In_ CPoint pt);
			bool OpenPopupMenuFromPoint(_In_ CPoint pt);
			bool ClosePopupMenuFromPoint(_In_ CPoint pt);

			bool IsExceptionMousePoint();

			DECLARE_MESSAGE_MAP()
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

			virtual BOOL OnUserToolTip(CMFCToolBarButton* pButton, CString& strTTText) const;

			void UpdateTooltips();

		public:
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
			afx_msg void OnCancelMode();

		protected:
			COLORREF m_clrToolBar[ERavidToolBarColorType_Count] = { 0, };
			COLORREF m_clrDisabledBtnException = RGB(240, 240, 240);

			bool m_bIsBottomEdgeDraw = false;

			long m_nTitleMinimumDisplayWidth = 0;
			long m_nTitleStartOffsetX = 0;
			long m_nButtonStartX = 0;

			ERavidToolBarButtonAlign m_eButtonAlign = ERavidToolBarButtonAlign_Left;

			static std::vector<std::pair<HWND, CWnd*> > m_vctCustomizePopupMenu;
			static std::vector<SRavidPopupMenuInfo> m_vctRavidPopupMenuInfo;
			static bool m_bLockAutoPopup;

			CRect m_crTitle = CRect(0, 0, 0, 0);

			CString m_strTitle = _T("");
			CString m_strDrawTitle = _T("");

			std::set<long> m_setIndexWhichHadPopupMenu;

			CMFCToolBarImages m_orgDisabledImagesLocked;

			long m_nHoverNumber = 0;
			long m_nHoverCount = 0;

			long m_nExceptionTopThickness = 0;

			CMFCToolBarButton* m_pOpenReadyButton = nullptr;

			std::set<long> m_setIndexAlwyasOn;

			long m_nObjectID = -1;
		};
	}
}