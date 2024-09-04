#pragma once

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidLedCtrl : public CStatic
		{
		// Construction
		public:
			CRavidLedCtrl();
			virtual ~CRavidLedCtrl();

		// Attributes
		public:

			COLORREF m_bOffColor[3];
			COLORREF m_bOnColor[3];
			COLORREF m_LedColor;

			void SetActivate(bool bActiv);
			bool GetActivate();


		// Operations
		public:

		// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(CRavidLedCtrl)
			//}}AFX_VIRTUAL

		// Implementation
		public:
			void SetText(CString strText);
			void SetStatus(BOOL bStatus);
			BOOL GetStatus();
			void Off();
			void On();

			BOOL m_bLedStatus;

		protected:
			
			// Generated message map functions
		protected:
			//{{AFX_MSG(CRavidLedCtrl)
			afx_msg void OnPaint();
			//}}AFX_MSG

			DECLARE_MESSAGE_MAP()

		protected:
			bool m_bActivate;
			CString m_strText;
		
		};
	}
}
