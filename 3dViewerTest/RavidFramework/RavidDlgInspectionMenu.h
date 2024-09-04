#pragma once


// CRavidDlgInspectionMenu 대화 상자입니다.
#include "RavidDialogBase.h"
#include "RavidTreeCtrl.h"

#include <vector>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidDlgInspectionMenu : public CRavidDialogBase
		{
			RavidPreventCopySelf(CRavidDlgInspectionMenu);

		public:
			DECLARE_DYNAMIC(CRavidDlgInspectionMenu)

		public:
			CRavidDlgInspectionMenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgInspectionMenu();

			virtual bool CreateMe(CWnd* pParent = nullptr);

		// 대화 상자 데이터입니다.
		// #ifdef AFX_DESIGN_TIME
		// 	enum { IDD = IDD_RAVID_DLG_INSPECTION_MENU };
		// #endif

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();

			afx_msg void OnPaint();

		public:
			bool SetInspectionMenu(_In_ std::vector<CString>* pVctInspectionMenu);
			bool GetInspectionMenu(_Out_ std::vector<CString>* pVctInspectionMenu);

			HTREEITEM AddInspectionMenu(_In_ CString strMenu, _In_opt_ HTREEITEM hAfterItem = TVI_LAST);
			bool DeleteInspectionMenu(_In_ HTREEITEM hItem);

			std::vector<HTREEITEM> GetSelectedInspectMenuTreeItems();

			bool GetInspectionMenuTreeUpCondition();
			bool GetInspectionMenuTreeDownCondition();

		protected:

			bool UpdateInspectionMenu();

			bool UpdateInspectionMenuTitle();

			bool UpdateButtonState();

		public:
			afx_msg void OnTvnSelchangedTreeRavidDlgInspectionMenu(NMHDR *pNMHDR, LRESULT *pResult);
			afx_msg void OnNMRClickTreeRavidDlgInspectionMenu(NMHDR *pNMHDR, LRESULT *pResult);
			afx_msg void OnBnClickedButtonRavidDlgInspectionMenuUp();
			afx_msg void OnBnClickedButtonRavidDlgInspectionMenuDown();

		protected:
			typedef struct sInspectionMenu
			{
				HTREEITEM hItem = 0;
				CString strMenu = _T("");
			}
			SInspectionMenu;

			HTREEITEM m_hRootItem = nullptr;

			CRavidTreeCtrl m_treeInspectionMenu;

			std::vector<SInspectionMenu> m_vctInspectionMenu;
			
			std::vector<SInspectionMenu>::iterator __InternalFindMenu(_In_ HTREEITEM hItem);
		};
	}
}
