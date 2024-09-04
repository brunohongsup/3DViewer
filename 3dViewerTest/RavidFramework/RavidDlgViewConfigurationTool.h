#pragma once

// CRavidDlgViewConfigurationTool 대화 상자입니다.
#include "RavidDialogBase.h"
#include "RavidTreeCtrl.h"

#include <memory>

namespace Ravid
{
	namespace Framework
	{
		class CRavidViewConfigurationToolSlider;
		class AFX_EXT_CLASS CRavidDlgViewConfigurationTool : public CRavidDialogBase
		{
		public:
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CRavidDlgViewConfigurationTool);
			DECLARE_DYNAMIC(CRavidDlgViewConfigurationTool);

			typedef struct sConfigurationInfo
			{
				HTREEITEM hItem = nullptr;

				long nNumber = -1;
				CString strTitle;
				std::vector<SConfigurationElementInfo*> vctElement;
			}SConfigurationInfo;

		public:
			CRavidDlgViewConfigurationTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgViewConfigurationTool();

			virtual bool CreateMe(CWnd* pParent = nullptr);

		// 대화 상자 데이터입니다.
		// #ifdef AFX_DESIGN_TIME
		// 	enum { IDD = IDD_RAVID_DLG_VIEW_CONFIGURATION_TOOL };
		// #endif

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();

			afx_msg void OnPaint();
			afx_msg void OnBnClickedButtonRavidDlgViewAdd();
			afx_msg void OnBnClickedButtonRavidDlgViewDelete();

			afx_msg void OnBnClickedButtonRavidDlgViewUp();
			afx_msg void OnBnClickedButtonRavidDlgViewDown();

			afx_msg void OnBnClickedButtonRavidDlgViewSave();

			afx_msg void OnTvnSelchangedTreeRavidDlgViewRegister(NMHDR *pNMHDR, LRESULT *pResult);
			afx_msg void OnNMDblclkTreeRavidDlgViewRegister(NMHDR *pNMHDR, LRESULT *pResult);

			afx_msg void OnClose();

		public:
			virtual bool Run();

			virtual bool SetSlider(_In_ CRavidViewConfigurationToolSlider* pSlider);

			virtual void SetChangedStatus(_In_ bool bChange);

		protected:

			virtual bool LoadData();

			virtual bool UpdateAvailableViewTree();
			virtual bool UpdateRegisterViewTree();

			virtual bool UpdateConfigurationTree();
			virtual bool UpdateElementTree();

			virtual bool UpdateSlider();

			std::vector<SConfigurationInfo>::iterator GetSelectedConfigurationInfo();

			virtual HTREEITEM AddRegisteredViewTree(_In_ SRegisterViewInfo* pInfo, _In_opt_ HTREEITEM hAfterItem = TVI_LAST);

			virtual std::vector<HTREEITEM> GetSelectedRegisteredViewTreeRoot();

			virtual HTREEITEM GetRegisteredViewTreeRoot(_In_ HTREEITEM hItem);
			virtual HTREEITEM GetRegisteredViewTreeItem(_In_ HTREEITEM hItem);

			virtual long GetLastViewObjectID(_In_ CString strClass);

			virtual bool UpdateRegisterViewObjectID();

			virtual bool GetRegisteredViewTreeUpCondition();
			virtual bool GetRegisteredViewTreeDownCondition();

			virtual bool UpdateButtonState();

			virtual CRect GetMainFrameRect();

			std::vector<SRegisterViewInfo*>::iterator FindInfo(_In_ HTREEITEM hItem);

			bool SortConfigurationElement();

			bool ClearRegisterViewInfoVector();
			bool ClearConfigurationInfoVector();

			bool SetRegisterViewMode(_In_ bool bSet);
			bool IsRegisterViewMode();

			bool Save();

		public:
			afx_msg void OnNMRClickTreeRavidDlgViewRegister(NMHDR *pNMHDR, LRESULT *pResult);
			afx_msg void OnBnClickedButtonRavidDlgViewChangeMode();
			afx_msg void OnNMRClickTreeRavidDlgViewAvailable(NMHDR *pNMHDR, LRESULT *pResult);
			afx_msg void OnTvnSelchangedTreeRavidDlgViewAvailable(NMHDR *pNMHDR, LRESULT *pResult);

		protected:
			bool m_bRegisterViewMode = true;

			CRavidViewConfigurationToolSlider* m_pSlider = nullptr;

			CRavidTreeCtrl m_treeAvailableViewList;
			CRavidTreeCtrl m_treeRegisterViewList;

			std::vector<SRegisterViewInfo*> m_vctRegisterView;

			std::vector<CRavidDlgViewConfigurationTool::SConfigurationInfo> m_vctConfiguration;

			static LPCTSTR m_lpszView[ERegisterViewType_Count];

			std::vector<CString> m_vctHighNode[ERegisterViewType_Count];
			bool m_bIsChanged = false;

		protected:
			virtual ptrdiff_t OnMessage(Ravid::Framework::CMessageBase* pMessage);
		};
	}
}
