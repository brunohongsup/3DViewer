
// RavidChildFrameBase.h : CRavidChildFrameBase 클래스의 인터페이스
//

#pragma once

namespace Ravid
{
	namespace Framework
	{
		class CRavidDlgViewConfigurationTool;
		class CRavidDlgConfigurationNew;

		class AFX_EXT_CLASS CRavidViewConfigurationToolSlider : public CWnd
		{
			DECLARE_DYNCREATE(CRavidViewConfigurationToolSlider)

			enum EMouseMode
			{
				EMouseMode_None = 0,
				EMouseMode_Body,
				EMouseMode_Left,
				EMouseMode_Top,
				EMouseMode_Right,
				EMouseMode_Bottom,
				EMouseMode_LeftTop,
				EMouseMode_RightTop,
				EMouseMode_RightBottom,
				EMouseMode_LeftBottom,
			};

		public:
			CRavidViewConfigurationToolSlider();
			virtual ~CRavidViewConfigurationToolSlider();

			bool Create(_In_ CWnd* pWndParent);

			bool RegisterWindowClass();

			void SetToolBarHeight(_In_ long nHeight);
			void SetToolBarBackgroundColor(_In_ COLORREF clr);

			void SetEdgeColor(_In_ COLORREF clr);
			void SetSelectedEdgeColor(_In_ COLORREF clr);

			void SetImageViewBackgroundColor(_In_ COLORREF clr);
			void SetFormViewBackgroundColor(_In_ COLORREF clr);

			void SetTitleColor(_In_ COLORREF clr);

			void SetTitleStartOffset(_In_ long nOffset);

			bool OnLButtonDown(_In_ CPoint pt);
			bool OnLButtonUp(_In_ CPoint pt);
			bool OnMouseMove(_In_ CPoint pt);

			bool OnSize();

			bool AddViewInfo(_In_ SConfigurationElementInfo* pInfo);
			bool DeleteViewInfo(_In_ SConfigurationElementInfo* pInfo);
			bool ClearViewInfo();

			bool SetViewManagerTool(_In_ CRavidDlgViewConfigurationTool* pDlg);
			bool SetViewManagerTool(_In_ CRavidDlgConfigurationNew* pDlg);

		protected:
			DECLARE_MESSAGE_MAP()

		public:
			afx_msg void OnPaint();
			afx_msg BOOL OnEraseBkgnd(CDC* pDC);

			LPCTSTR GetCursorType();

			bool AssignPositionRect(_In_ SConfigurationElementInfo* pInfo);
			bool AssignRatio(_In_ SConfigurationElementInfo* pInfo);
			bool AssignCoordinate(_In_ SConfigurationElementInfo* pInfo);

		protected:

			bool SetToolChangedStatus(_In_ bool bChange);

			SConfigurationElementInfo* GetViewInfo(_In_ CPoint pt);

			EMouseMode GetMouseMode(_In_ CRect crView, _In_ CPoint pt);

			bool SetCursor(_In_ EMouseMode eMode);

		private:
			CRavidDlgViewConfigurationTool* m_pDlgViewConfigurationTool = nullptr;
			CRavidDlgConfigurationNew* m_pDlgConfigurationNew = nullptr;

			CCriticalSection m_cs;

			std::vector<SConfigurationElementInfo*> m_vctViewInfo;
			SConfigurationElementInfo* m_pSelectedViewInfo = nullptr;
			
			CPoint m_ptChangeDiff;
			EMouseMode m_eMouseMode = EMouseMode_None;

			HBRUSH m_hBrBackground = 0;

			long m_nToolBarHeight = 0;
			COLORREF m_clrToolBarBackground = 0;

			COLORREF m_clrEdge = 0;
			COLORREF m_clrSelectedEdge = 0;

			COLORREF m_clrImageViewBackground = 0;
			COLORREF m_clrFormViewBackground = 0;

			COLORREF m_clrTitle = 0;
			long m_nTitleStartOffset = 0;

			LPCTSTR m_lpszCursor = IDC_ARROW;

			CSize m_szMinimum = CSize(50, 50);
		};
	}
}
