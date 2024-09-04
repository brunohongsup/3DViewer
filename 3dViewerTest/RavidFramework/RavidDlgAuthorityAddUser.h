#pragma once

// CRavidDlgAuthorityAddUser 대화 상자입니다.

#include "RavidDlgInputBox.h"

namespace Ravid
{
	namespace Framework
	{
		class CRavidInputBoxInfo;

		class AFX_EXT_CLASS CRavidDlgAuthorityAddUser : public CRavidDlgInputBox
		{
			RavidPreventCopySelf(CRavidDlgAuthorityAddUser);
		public:
			DECLARE_DYNAMIC(CRavidDlgAuthorityAddUser)

		public:
			CRavidDlgAuthorityAddUser(_In_ CRavidInputBoxInfo* pRavidInputBoxInfo, _In_opt_ CWnd* pParent = nullptr);
			virtual ~CRavidDlgAuthorityAddUser();

			virtual bool CreateMe(CWnd* pParent = nullptr);

		// 대화 상자 데이터입니다.
		// #ifdef AFX_DESIGN_TIME
		// 	enum { IDD = IDD_RAVID_DLG_AUTHORITY_ADD_USER };
		// #endif

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();

			afx_msg void OnCbnSelchangeUserType();

		protected:
			virtual bool UpdateInputBox() override;

			virtual bool UpdateControl();
		};
	}
}
