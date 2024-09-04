#pragma once


// CRavidDlgFrameworkConfigurationTool 대화 상자입니다.

#include "RavidDialogBase.h"

class CRavidTreeView;
class CRavidTreeViewItem;

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidDlgFrameworkConfigurationTool : public CRavidDialogBase
		{
		public:

			enum ETreeItemType
			{
				ETreeItemType_None = 0,
				ETreeItemType_Edit,
				ETreeItemType_Combo,
			};

			DECLARE_DYNAMIC(CRavidDlgFrameworkConfigurationTool)

			RavidPreventCopySelf(CRavidDlgFrameworkConfigurationTool);

		public:
			CRavidDlgFrameworkConfigurationTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgFrameworkConfigurationTool();

			virtual bool CreateMe(CWnd* pParent = nullptr);

		// 대화 상자 데이터입니다.
		// #ifdef AFX_DESIGN_TIME
		// 	enum { IDD = IDD_RAVID_DLG_FRAMEWORK_CONFIGURATION_TOOL };
		// #endif

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();

			afx_msg void OnPaint();

			afx_msg void OnBnClickedButtonFrameworkConfigurationToolSave();
			
			afx_msg void OnClose();

		public:
			virtual bool UpdateControls() override;

			virtual bool UpdateTree();

			virtual bool SetSheetSkin();

		protected:
			virtual bool Save();

			virtual CRavidTreeViewItem* AddTreeItem(_In_ ETreeItemType eType, _In_ CString strTitle, _In_opt_ CString strDefaultData = _T(""), _In_opt_ CRavidTreeViewItem* pParentTreeItem = nullptr, _In_opt_ std::vector<CString>* pVctComboList = nullptr);

		protected:
			virtual ptrdiff_t OnMessage(Ravid::Framework::CMessageBase* pMessage);

		protected:
			enum EParameterType
			{
				EParameterType_DeviceItemEdit = 0,
				EParameterType_DeviceControl,
				EParameterType_ModelItemEdit,
				EParameterType_ModelOpen,
				EParameterType_ModelClose,
				EParameterType_ModelNew,
				EParameterType_ModelCopy,
				EParameterType_ModelDelete,
				EParameterType_ModelConfigurationAccess,
				EParameterType_ModelConfigurationNew,
				EParameterType_ModelConfigurationModify,
				EParameterType_ModelConfigurationDelete,
				EParameterType_ModelConfigurationDuplicate,
				EParameterType_ImageProcessingToolRun,
				EParameterType_ConfigurationSave,
				EParameterType_ConfigurationSaveas,
				EParameterType_ConfigurationNew,
				EParameterType_ConfigurationDelete,
				EParameterType_ConfigurationRename,
				EParameterType_MainFrameRelativeLayout,
				EParameterType_ChildFrameSize,
				EParameterType_ImageViewAccuracyEdit,
				EParameterType_ImageViewAccuracyX,
				EParameterType_ImageViewAccuracyY,
				EParameterType_ImageViewLayer,
				EParameterType_ImageViewScrollBar,
			};

			typedef struct sParameterInfo
			{
				EParameterType eType = EParameterType_DeviceItemEdit;
				long nOptIndex1 = 0;
				long nOptIndex2 = 0;

				CRavidTreeViewItem* pItem = nullptr;

				void Clear()
				{
					eType = EParameterType_DeviceItemEdit;
					nOptIndex1 = 0;
					nOptIndex2 = 0;

					pItem = nullptr;
				}
			}
			SParameterInfo;

			CRavidTreeView* m_pTreeParam = nullptr;

			std::vector<SParameterInfo> m_vctParameterInfo;

			bool m_bIsChanged = false;

		};
	}
}
