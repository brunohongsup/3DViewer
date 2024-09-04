#pragma once

// CRavidDlgTeachingMenu 대화 상자입니다.
#include "RavidDialogBase.h"
#include "RavidTreeCtrl.h"

#include <memory>
#include <map>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidDlgTeachingMenu : public CRavidDialogBase
		{
			enum ETreeItemType
			{
				ETreeItemType_None = 0,
				ETreeItemType_Root,
				ETreeItemType_Menu,
				ETreeItemType_Element,
			};

		public:
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CRavidDlgTeachingMenu);

			DECLARE_DYNAMIC(CRavidDlgTeachingMenu)

		public:
			CRavidDlgTeachingMenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgTeachingMenu();

			virtual bool CreateMe(CWnd* pParent = nullptr);

		// 대화 상자 데이터입니다.
		// #ifdef AFX_DESIGN_TIME
		// 	enum { IDD = IDD_RAVID_DLG_TEACHING_MENU };
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
			bool SetMenu(_In_ std::vector<SRegisterTeachingMenuInfo>* pVctTeachingMenu);
			bool GetMenu(_Out_ std::vector<SRegisterTeachingMenuInfo>* pVctTeachingMenu);

			HTREEITEM AddMenu(_In_ SRegisterTeachingMenuInfo sInfo, _In_opt_ HTREEITEM hAfterItem = TVI_LAST);
			bool DeleteMenu(_In_ HTREEITEM hItem);

			HTREEITEM AddElement(_In_ SRegisterTeachingMenuElement sElement, _In_ HTREEITEM hParentItem, _In_opt_ HTREEITEM hAfterItem = TVI_LAST);
			bool DeleteElement(_In_ HTREEITEM hItem);

			std::vector<HTREEITEM> GetSelectedMenuTreeItems();

			bool GetMenuTreeUpCondition();
			bool GetMenuTreeDownCondition();

		protected:

			bool UpdateMenu();

			bool UpdateTreeItemTitle();

			bool UpdateButtonState();

			std::vector<SRegisterTeachingMenuInfo>::iterator FindMenu(_In_ HTREEITEM hItem);

			std::vector<SRegisterTeachingMenuElement>::iterator FindElement(_In_ HTREEITEM hItem);

			CString ConvertGeometryShapeTypeEnumToString(_In_ ERavidGeometryShapeType eType);
			CString ConvertGeometryShapeTypeIndexToString(_In_ int nIndex);

			ERavidGeometryShapeType ConvertGeometryShapeTypeStringToEnum(_In_ CString strType);
			ERavidGeometryShapeType ConvertGeometryShapeTypeIndexToEnum(_In_ int nIndex);

			int ConvertGeometryShapeTypeEnumToIndex(_In_ ERavidGeometryShapeType eType);


			CString ConvertGeometryTemplateTypeEnumToString(_In_ ERavidGeometryTemplateType eType);
			CString ConvertGeometryTemplateTypeIndexToString(_In_ int nIndex);

			ERavidGeometryTemplateType ConvertGeometryTemplateTypeStringToEnum(_In_ CString strType);
			ERavidGeometryTemplateType ConvertGeometryTemplateTypeIndexToEnum(_In_ int nIndex);

			int ConvertGeometryTemplateTypeEnumToIndex(_In_ ERavidGeometryTemplateType eType);

			bool RunMenuInputBox(_In_ CString strInputBoxTitle, _Inout_ SRegisterTeachingMenuInfo& sInfo);

			bool RunElementInputBox(_In_ CString strInputBoxTitle, _Inout_ SRegisterTeachingMenuElement& sElement);

			ETreeItemType GetTreeItemType(_In_ HTREEITEM hItem);

			CString GetElementTitle(_In_ int nIndex, _In_ SRegisterTeachingMenuElement& sElement);

		public:
			afx_msg void OnTvnSelchangedTreeRavidDlgTeachingMenu(NMHDR *pNMHDR, LRESULT *pResult);
			afx_msg void OnNMRClickTreeRavidDlgTeachingMenu(NMHDR *pNMHDR, LRESULT *pResult);
			afx_msg void OnBnClickedButtonRavidDlgTeachingMenuUp();
			afx_msg void OnBnClickedButtonRavidDlgTeachingMenuDown();

		protected:
			HTREEITEM m_hRootItem = nullptr;

			CRavidTreeCtrl m_treeMenu;

			std::vector<SRegisterTeachingMenuInfo> m_vctMenu;

			std::map<ERavidGeometryShapeType, CString> m_mapShapeTypeTable;
			std::map<ERavidGeometryTemplateType, CString> m_mapTemplateTypeTable;
		};
	}
}
