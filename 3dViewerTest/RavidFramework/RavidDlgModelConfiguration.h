#pragma once


// CRavidDlgModelConfiguration 대화 상자입니다.

#include "RavidDialogBase.h"
#include "RavidSheet.h"

#include <memory>

class CRavidTreeViewItemStatic;

namespace Ravid
{
	namespace Database
	{
		class CRavidDatabase;
		class CRavidRecordset;
	}

	namespace Framework
	{
		class CRavidDlgModelConfigurationSetting;
		class AFX_EXT_CLASS CRavidDlgModelConfiguration : public CRavidDialogBase
		{
			enum EButton
			{
				EButton_New = 0,
				EButton_Modify,
				EButton_Delete,
				EButton_Duplicate,
				EButton_Show,
				EButton_Count,
			};

		public:
			DECLARE_DYNAMIC(CRavidDlgModelConfiguration)

		public:
			CRavidDlgModelConfiguration(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgModelConfiguration();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual bool CreateMe(CWnd* pParent = nullptr);

		// 대화 상자 데이터입니다.
		// #ifdef AFX_DESIGN_TIME
		// 	enum { IDD = IDD_RAVID_DLG_MODEL_CONFIGURATION };
		// #endif

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();

			afx_msg void OnNMSheetBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnNMSheetEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnNMSheetCancelLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnNMSheetSelchanged(NMHDR* pNMHDR, LRESULT* pResult);

			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationNew();
			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationModify();
			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationDelete();
			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationDuplicate();
			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationShowStruct();

		public:
			bool UpdateModelTypeList();

			bool UpdateButtonStatus();

		protected:
			bool InitSheet();

			CString GetSelectedModelTypeName();
			CString GetSelectedModelDescription();

		protected:
			bool m_bIsSheetEditMode = false;
			CRavidSheet m_sheetModelTypeList;

			bool m_bIsModelSettingVisible = false;
			CRavidDlgModelConfigurationSetting* m_pModelSetting = nullptr;

			bool m_bIsDeleteEnable = true;
			bool m_bIsDuplicateEnable = true;
			bool m_bIsModifyEnable = true;
		};
	}
}
