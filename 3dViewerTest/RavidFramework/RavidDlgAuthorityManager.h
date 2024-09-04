#pragma once


// CRavidDlgAuthorityManager 대화 상자입니다.

#include "RavidDialogBase.h"
#include "RavidSheet.h"
#include <memory>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidDlgAuthorityManager : public CRavidDialogBase
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CRavidDlgAuthorityManager);

		public:
			DECLARE_DYNAMIC(CRavidDlgAuthorityManager)

			enum EAuthoritySheetTitle
			{
				EAuthoritySheetTitle_Name = 0,
				EAuthoritySheetTitle_Type,
				EAuthoritySheetTitle_Status,
				EAuthoritySheetTitle_Count,
			};

			enum EFunc
			{
				EFunc_Add = 0,
				EFunc_ModifyPassword,
				EFunc_ModifyUserType,
				EFunc_Delete,
				EFunc_Count,
			};

		public:
			CRavidDlgAuthorityManager(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgAuthorityManager();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual bool CreateMe(CWnd* pParent = nullptr);

		// 대화 상자 데이터입니다.
		// #ifdef AFX_DESIGN_TIME
		// 	enum { IDD = IDD_RAVID_DLG_AUTHORITY_MANAGER };
		// #endif

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnClose();

			afx_msg void OnBnClickedBtnRavidDlgAuthorityAdd();
			afx_msg void OnBnClickedBtnRavidDlgAuthorityModifyPassword();
			afx_msg void OnBnClickedBtnRavidDlgAuthorityModifyUserType();
			afx_msg void OnBnClickedBtnRavidDlgAuthorityDelete();

			afx_msg void OnNMSheetSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnNMSheetBeginEdit(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnNMSheetEndEdit(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnNMSheetCancelEdit(NMHDR* pNMHDR, LRESULT* pResult);

		public:
			bool UpdateAuthorityList();

		protected:
			bool IsDeveloperKey();

			bool ConvertUserTypeEnumToString(_In_ Ravid::Framework::EUserType eUserType, _Out_ CString& strUserType);
			bool ConvertUserTypeStringToEnum(_In_ CString strUserType, _Out_ Ravid::Framework::EUserType& eUserType);

			bool GetCondition(_In_ EFunc eFunc);

			CString GetUserTypeComboList(_In_ bool bCheckDeveloperKey);

			CString GetSelectedUserName();
			Ravid::Framework::EUserType GetSelectedUserType();

			bool InitSheet();
			bool RecalcSheet();

			bool UpdateButtonStatus();

			bool ShowMessage(_In_ CString strFunction, _In_ Ravid::Framework::EAuthResult eResult);

		protected:
			double m_dblTitleSizeRatio[CRavidDlgAuthorityManager::EAuthoritySheetTitle_Count];
			CRavidSheet m_sheetAuthority;

		};
	}
}
