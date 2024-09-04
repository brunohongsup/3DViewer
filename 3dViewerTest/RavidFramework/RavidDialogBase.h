
// RavidDialogBaseBase.h : RavidDialogBase 클래스의 인터페이스
//

#pragma once

#include "RavidObjectEx.h"

#include "../RavidCore/RavidRect.h"
#include <vector>
#include <queue>
#include <map>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidDialogBase : public CDialog, public CRavidObjectEx
		{
			RavidUseDynamicCreation();

			DECLARE_DYNAMIC(CRavidDialogBase)

		public:
			enum ERavidDialogCaptionButton
			{
				ERavidDialogCaptionButton_Close = 0,
				ERavidDialogCaptionButton_Minimum,
				ERavidDialogCaptionButton_Count,
			};

			CRavidDialogBase();
			CRavidDialogBase(const CRavidDialogBase& rd) { }
			CRavidDialogBase(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
			CRavidDialogBase(UINT nIDTemplate, CWnd* pParentWnd = NULL);
			virtual ~CRavidDialogBase();

			virtual bool CreateMe(CWnd* pParent = nullptr);

		public:
			virtual bool UpdateControls();

			DECLARE_MESSAGE_MAP()
			virtual BOOL OnInitDialog();
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnPaint();
			afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
			afx_msg BOOL OnEraseBkgnd(CDC* pDC);
			afx_msg void OnNcPaint();
			afx_msg BOOL OnNcActivate(BOOL bActive);
			afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
			afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
			afx_msg void OnDestroy();
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

			void EnableWindowSize(_In_ bool bEnable);
			bool IsEnabledWindowSize();

			void EnableAdaptiveScale(_In_ bool bEnable);
			bool IsEnabledAdaptiveScale();

			void SetAdaptiveScaleAlign(_In_ EAdaptiveScaleAlign eAlign);
			EAdaptiveScaleAlign GetAdaptiveScaleAlign();

			void SetEdgeThickness(_In_ long nThickness);
			long GetEdgeThickness();


		public:
			double GetScale(HWND hWnd);

			bool SetFrameColor(_In_ ERavidFrameColorType eRFCT, _In_ COLORREF clr);
			COLORREF GetFrameColor(_In_ ERavidFrameColorType eRFCT);

			bool SetCaptionColor(_In_ ERavidCaptionColorType eRCCT, _In_ COLORREF clr);
			COLORREF GetCaptionColor(_In_ ERavidCaptionColorType eRCCT);

			bool SetControlColor(_In_ ERavidControlColorType eRCCT, _In_ COLORREF clr);
			COLORREF GetControlColor(_In_ ERavidControlColorType eRCCT);

			virtual bool FitDialog();

			virtual bool SetWindowRatio(_In_ double dblWidthRatio, _In_ double dblHeightRatio);

			void SetWindowSizeRatioMode(_In_ bool bSet);
			bool IsWindowSizeRatioMode();

			bool SetDefaultDialogRect(_In_ CRavidRect<int> rr);

			bool AddComboBoxControlID(_In_ UINT uiID);

		protected:
			virtual bool OnCaptionButtonClose();
			virtual bool OnCaptionButtonMinimum();

			virtual bool AdjustWindowRegion();

			virtual bool InsertAdaptiveScaleInfo(_In_ CWnd* pWnd);

			virtual bool AddThreadDisplayer(_In_ TViewThreadDisplayer* pDisp);
			virtual TViewThreadDisplayer* GetDisplayer();
			virtual bool IsDisplayerExisting();
			virtual bool RunThreadDisplayer();
			virtual void ClearDisplayer();

		protected:
			bool m_bEnabledAdaptiveScale = true;

			bool m_bIsDefaultDlgSet = false;
			bool m_bIsDefaultControlSet = false;
			CRect m_crDefaultWindow = CRect(0, 0, 0, 0);

			CSize m_szPreWindow = CSize(0, 0);

			bool m_bIsWindowSizeRatioMode = false;

			std::map<HWND, SRavidDialogAdaptiveScaleInfo> m_mapCtrlAdaptiveScaleInfos;

			SRavidCaptionButtonInfo m_sCaptionButtonInfos[ERavidDialogCaptionButton_Count];

			HBRUSH m_hBackgroundBrush = nullptr;

			COLORREF m_clrFrame[ERavidFrameColorType_Count] = { 0, };
			COLORREF m_clrCaption[ERavidCaptionColorType_Count] = { 0, };
			COLORREF m_clrControl[ERavidControlColorType_Count] = { 0, };
			COLORREF m_clrBackupBackground = 0;

			bool m_bWindowDrag = false;
			CPoint m_ptDragPoint = CPoint(0, 0);

			long m_nEdgeThickness = 1;

			std::queue<TViewThreadDisplayer*> m_queDisplayer;

			std::vector<UINT> m_vctComboBoxControlID;

			EAdaptiveScaleAlign m_eAdaptiveScaleAlign = EAdaptiveScaleAlign_Center;

			bool m_bIsbIsActivate = true;
		};
	}
}
