#pragma once


// CRavidDlgModelConfigurationSetting 대화 상자입니다.

#include "RavidDialogBase.h"

#include <memory>

class CRavidTreeView;
class CRavidTreeViewItem;

namespace Ravid
{
	namespace Database
	{
		class CRavidDatabase;
		class CRavidRecordset;
	}

	namespace Framework
	{
		class CModelConfigurationInfo;
		class CRavidSheetProperty;

		class AFX_EXT_CLASS CRavidDlgModelConfigurationSetting : public CRavidDialogBase
		{

		public:
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CRavidDlgModelConfigurationSetting);
			DECLARE_DYNAMIC(CRavidDlgModelConfigurationSetting)

		public:
			CRavidDlgModelConfigurationSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgModelConfigurationSetting();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual bool CreateMe(CWnd* pParent = nullptr);

		// 대화 상자 데이터입니다.
		// #ifdef AFX_DESIGN_TIME
		// 	enum { IDD = IDD_RAVID_DLG_MODEL_CONFIGURATION_SETTING };
		// #endif

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();

			afx_msg void OnTreeItemRButtonUp(NMHDR* pNotifyStruct, LRESULT* plResult);
			afx_msg void OnTreeSelChange(NMHDR* pNotifyStruct, LRESULT* plResult);
			afx_msg void OnTreeLButtonDblClk(NMHDR* pNotifyStruct, LRESULT* plResult);
			afx_msg void OnTreeLButtonUp(NMHDR* pNotifyStruct, LRESULT* plResult);
			afx_msg void OnTreeVerticalScroll(NMHDR* pNotifyStruct, LRESULT* plResult);

			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationSettingUp();
			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationSettingDown();

			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationSettingSave();
			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationSettingCancel();
			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationSettingAddParameterStruct();

			afx_msg void OnDeltaposSpinRavidDlgModelConfigurationSettingModelConcurrentSize(NMHDR *pNMHDR, LRESULT *pResult);

			afx_msg void OnNMSheetEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);

			afx_msg BOOL OnEraseBkgnd(CDC* pDC);

		public:
			void RegisterModelConfigurationInfo(_In_ CModelConfigurationInfo* pRMCI);
			CModelConfigurationInfo* GetModelConfigurationInfo();

			bool IsEmptyUniqueID(_In_ unsigned long ulUniqueID);

			bool IsBeingSameVariableName(_In_ CString strVariableName);

			bool CheckVariableNameAvailability(_In_ CString strVariableName);

			bool DevideVariable(_Inout_ CString& strVariable, _Out_ long& nArraySize);

			bool InitPropertyTree();
			bool AddPropertyTree();
			bool m_bIsSheetEditMode = false;

		protected:
			bool FoundationWorkTree();

			bool RegisterTree(_In_ SModelElementProperty* pMEP, _In_opt_ bool bLocationUp = false, _In_opt_ SModelElementProperty* pLocationMEP = nullptr, _In_opt_ bool bUpdate = true);

			bool AddTreeItem(_In_ SModelElementProperty* pCurMEP, _In_opt_ bool bLocationUp = false, _In_opt_ SModelElementProperty* pLocationMEP = nullptr, _In_opt_ bool bUpdate = true);

			bool IsChildItem(_In_ SModelElementProperty* pCurMEP, _In_ SModelElementProperty* pCompareMEP);

			std::vector<SModelElementProperty*> FilterChildItemProperty(_In_ std::vector<CRavidTreeViewItem*>& vctItems);
			bool GetChildItemProperty(_In_ SModelElementProperty* pParentProperty, _Out_ std::vector<SModelElementProperty*>* pVctMEP);

			SModelElementProperty* GetElementProperty(_In_ CRavidTreeViewItem* pItem);

			bool UpdateSettingButtonStatus();

			bool CheckModelTypeNameAvailability();

		public:
			bool ReviseInvalidateTree(_In_ SModelElementProperty* pMEP);
			bool ReviseInvalidateDepth(_In_ SModelElementProperty * pParentMEP, _In_ SModelElementProperty * pChildMEP);

			CRavidTreeViewItem * GetLastSibling(_In_ CRavidTreeViewItem *pSibling);
			CRavidTreeViewItem * GetFindSibling(_In_ CRavidTreeViewItem *pSibling, _In_ CRavidTreeViewItem *pTargetSibling);

			bool CheckChildProperty(CRavidTreeViewItem *pItem);
			bool InitSheet();

			bool UpdateScrollSheetToTree();
			bool UpdateScrollTreeToSheet();

			bool SetModifyAuthorityItem(_In_ SModelElementProperty* pMEP, _In_ EUserType eAuthority);

		protected:
			virtual void OnOK();

		protected:
			CRavidTreeView* m_pOTSetting = nullptr;
			CRavidTreeViewItem* m_pTreeRootItem = nullptr;
			CModelConfigurationInfo* m_pMCI = nullptr;
			CButton m_btnSettingUp;
			CButton m_btnSettingDown;
			CString m_strOrgModelTypeName = _T("");

			std::vector<SModelElementProperty*> m_vctCopyMEP;

			CRavidSheetProperty *m_psheetSetting = nullptr;
			sModelElementProperty* m_pMEPOrg = nullptr;

			int m_nSheetVScrollPos;
			int m_nTreeVScrollPos;
		};
	}
}
