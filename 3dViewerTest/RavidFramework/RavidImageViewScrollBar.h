#pragma once

namespace Ravid
{
	namespace Framework
	{
		class CRavidChildFrameBase;
		class CRavidImageView;

		class AFX_EXT_CLASS CRavidImageViewScrollBar : public CScrollBar
		{
		public:
			RavidPreventCopySelf(CRavidImageViewScrollBar);

			CRavidImageViewScrollBar();
			virtual ~CRavidImageViewScrollBar();

			virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

			virtual BOOL PreTranslateMessage(MSG* pMsg);

			virtual void SetVScrollSize(_In_ int nSize);
			virtual void SetHScrollSize(_In_ int nSIze);

			virtual void SetVScrollPos(_In_ int nPos);
			virtual void SetHScrollPos(_In_ int nPos);

			DECLARE_MESSAGE_MAP()
			afx_msg void OnTimer(UINT_PTR nIDEvent);

		protected:
			virtual void OffTimer();

		public:

			bool m_bLButtonDown = false;
			bool m_bIsMouseInBar = false;

			int m_nVScrollPos = 0;
			int m_nHScrollPos = 0;

			int m_nVScrollSize = 0;
			int m_nHScrollSize = 0;

			int m_nMousePos = 0;

			bool m_bVScrollBar = false;
			bool m_bHScrollBar = false;

			bool m_bTimer = false;

			bool m_bMouseIsRightOfBar = false;
			bool m_bMouseIsLeftOfBar = false;

			bool m_bFirstClick = true;

			LPARAM m_lParamMouse = 0;
		};
	}
}

