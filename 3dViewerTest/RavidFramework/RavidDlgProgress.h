#pragma once

#include "RavidDialogBase.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidDlgProgress : public CRavidDialogBase
		{
			RavidPreventCopySelf(CRavidDlgProgress);

		public:
			DECLARE_DYNAMIC(CRavidDlgProgress);

		public:
			CRavidDlgProgress(_In_ CString strMessaage, _In_ double dblMaxValue, _In_opt_ ERavidModalType eModalType, _In_opt_ COLORREF clrTextColor, _In_opt_ bool bShowProgressBar, _In_opt_ COLORREF clrProgressBarColor, _In_opt_ CBitmap* pBmpBackground, _In_opt_ int nAlphaBlendValue);
			virtual ~CRavidDlgProgress();

#ifdef AFX_DESIGN_TIME
#endif
			virtual bool Create();

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);

			DECLARE_MESSAGE_MAP();
		public:
			virtual BOOL OnInitDialog();

		protected:
			virtual bool InitializeProgressDialog();
			CRect MonitorSize();

		public:
			bool UpdateMessage(_In_ CString strMessage);
			bool UpdateProgressBar(_In_ double dblState);
			bool UpdateMessageAndBar(_In_ CString strMessage, _In_ double dblState);
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			void DrawProgressBar();
			bool SetInitialDialogSize();
			afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
			afx_msg BOOL OnEraseBkgnd(CDC* pDC);
			bool GetIsModal();
			void SetIsModal(_In_ bool bIsModal);

		protected:
			bool m_bIsModal = false;
			CString m_strTitle = _T("");
			CString m_strMessage = _T("");
			double m_dblMaxValue = 100;
			double m_dblCurrentState = 0;
			bool m_bShowProgressBar = true;

			CFont m_fontMessage;

			COLORREF m_clrTextColor = RGB(0, 0, 0);
			COLORREF m_clrBackground = RGB(240, 240, 240);
			COLORREF m_clrProgressBar = RGB(0, 0, 0);

			CBitmap* m_pImage = nullptr;

			double m_dblInitialDlgHeight = 0;
			double m_dblInitialDlgWidth = 0;
			double m_dblResize = 1;
			bool m_bSetDefaultImage = false;

			int m_nAlphaBlendValue = 127;

		};
	}
}