
// RavidFormViewBase.h : RavidFormViewBase 클래스의 인터페이스
//

#pragma once

#include "RavidObjectEx.h"

#include <vector>
#include <queue>
#include <map>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidFormViewBase : public CFormView, public CRavidObjectEx
		{
			RavidUseDynamicCreation();

		public:
			CRavidFormViewBase();
			CRavidFormViewBase(const CRavidFormViewBase& rfv);
			virtual ~CRavidFormViewBase();

		protected:
			CRavidFormViewBase(LPCTSTR lpszTemplateName);
			CRavidFormViewBase(UINT nIDTemplate);
			virtual void OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);

		public:
			DECLARE_MESSAGE_MAP()
			virtual void OnInitialUpdate();
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnPaint();
			afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
			afx_msg BOOL OnEraseBkgnd(CDC* pDC);
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnDestroy();

		public:
			virtual bool SetTitle(_In_ CString strTitle);
			CString GetTitle();

			bool SetControlColor(_In_ ERavidControlColorType eRCCT, _In_ COLORREF clr);
			COLORREF GetControlColor(_In_ ERavidControlColorType eRCCT);

			void SetAdaptiveScaleAlign(_In_ EAdaptiveScaleAlign eAlign);
			EAdaptiveScaleAlign GetAdaptiveScaleAlign();

			bool AddComboBoxControlID(_In_ UINT uiID);

		protected:
			bool AddThreadDisplayer(_In_ TViewThreadDisplayer* pDisp);
			TViewThreadDisplayer* GetDisplayer();
			bool IsDisplayerExisting();
			bool RunThreadDisplayer();
			void ClearDisplayer();

		protected:
			std::queue<TViewThreadDisplayer*> m_queDisplayer;

			bool m_bIsDefaultFormViewSet = false;
			bool m_bIsDefaultControlSet = false;
			CRect m_crDefaultWindow = CRect(0, 0, 0, 0);

			CSize m_szPreWindow = CSize(0, 0);

			std::map<HWND, SRavidDialogAdaptiveScaleInfo> m_mapCtrlAdaptiveScaleInfos;

			HBRUSH m_hBackgroundBrush = nullptr;

			COLORREF m_clrControl[ERavidControlColorType_Count] = { 0, };
			COLORREF m_clrBackupBackground = 0;

			EAdaptiveScaleAlign m_eAdaptiveScaleAlign = EAdaptiveScaleAlign_Center;

			std::vector<UINT> m_vctComboBoxControlID;
		};
	}
}

