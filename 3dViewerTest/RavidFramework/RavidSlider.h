
// RavidChildFrameBase.h : CRavidChildFrameBase 클래스의 인터페이스
//

#pragma once

namespace Ravid
{
	namespace Framework
	{
		class CRavidChildFrameBase;
		
		class AFX_EXT_CLASS CRavidSlider : public CWnd
		{
			DECLARE_DYNCREATE(CRavidSlider)

		public:
			CRavidSlider();
			virtual ~CRavidSlider();

			bool Create(_In_ CWnd* pWndParent);

			bool RegisterWindowClass();

			bool SetFrameRect(_In_ CRect cr);
			CRect GetFrameRect();

			void SetMinimumFrameSize(_In_ CSize sz);
			CSize GetMinimumFrameSize();

			bool OffsetFrameRect(_In_ CPoint ptOffset);

			void SetFrameSizeChangeMode(_In_ ERavidFrameSizeChangeMode eFSCM);
			ERavidFrameSizeChangeMode GetFrameSizeChangeMode();

			void SetToolBarHeight(_In_ long nHeight);
			void SetToolBarBackgroundColor(_In_ COLORREF clr);
			void SetEdgeColor(_In_ COLORREF clr);
			void SetViewBackgroundColor(_In_ COLORREF clr);

			void SetRelativeFrameDisplay(_In_ bool bSet);
			bool GetRelativeFrameDisplay();

		protected:
			DECLARE_MESSAGE_MAP()

		public:
			afx_msg void OnPaint();
			afx_msg BOOL OnEraseBkgnd(CDC* pDC);

		protected:
			ERavidFrameSizeChangeMode m_eFrameSizeChangeMode = ERavidFrameSizeChangeMode_None;

			CRect m_crFrameRef = CRect(0, 0, 0, 0);
			CRect m_crFrameDraw = CRect(0, 0, 0, 0);

			CSize m_szFrameMinimum = CSize(0, 0);

			HBRUSH m_hBrBackground = 0;

			CRavidChildFrameBase* m_pParentChildFrame = nullptr;

			long m_nToolBarHeight = 0;
			COLORREF m_clrToolBarBackground = 0;
			COLORREF m_clrEdge = 0;
			COLORREF m_clrViewBackground = 0;

			bool m_bRelativeFrameDisplay = false;
		};
	}
}
