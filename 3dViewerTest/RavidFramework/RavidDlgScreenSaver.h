#pragma once

// CRavidDlgScreenSaver 대화 상자입니다.

#include "RavidDialogBase.h"

#include <memory>

namespace Ravid
{
	namespace Framework
	{
		class CRavidDlgScreenSaver : public CRavidDialogBase
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CRavidDlgScreenSaver);
			DECLARE_DYNAMIC(CRavidDlgScreenSaver)

		public:
			CRavidDlgScreenSaver(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgScreenSaver();

			virtual bool CreateMe(CWnd* pParent = nullptr);

			virtual bool SetTitle(_In_ CString strTitle);
			virtual bool SetWaitingTime(_In_ DWORD dwTimeMS);
			virtual bool UseLogIn(_In_ bool bUsed);

			virtual bool Run();

		protected:
			virtual bool ShowControls(_In_ bool bShow);
			virtual bool SetPosition();

			virtual CString GetKeyFromPoint(_In_ CPoint pt);

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual BOOL OnInitDialog();
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			afx_msg BOOL OnEraseBkgnd(CDC* pDC);

			static LRESULT CALLBACK WinKeyHook(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam); 
			
		public:
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnBnClickedButtonScreenSaverLogIn();
			afx_msg void OnClose();
			afx_msg void OnDestroy();
			afx_msg void OnPaint();
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
			afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

		protected:
			CStatic m_staticTitle;
			CStatic m_staticMessage;
			CStatic m_staticLogInGroup;
			CStatic m_staticID;
			CStatic m_staticPW;
			CEdit m_editID;
			CEdit m_editPW;
			CButton m_buttonLogIn;

			CStatic m_staticKeyboard;

			CString m_strTitle = _T("");
			DWORD m_dwWaitingTime = 10000;
			bool m_bUsedLogIn = false;

			CRect m_crKeyboard;

			std::vector<std::pair<CString, CRect>> m_vctKeyboard;

			double m_dblKeySize;

			bool m_bIsLbuttonDown;

			CString m_strLButtonDownKey;

			HHOOK m_hHook = nullptr;
		};
	}
}