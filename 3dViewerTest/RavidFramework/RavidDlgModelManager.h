#pragma once


// CRavidDlgModelManager 대화 상자입니다.

#include "RavidDialogBase.h"
#include "RavidTreeView.h"
#include "RavidSheet.h"

#include <memory>

class CRavidTreeView;

namespace Ravid
{
	namespace Framework
	{
		class CModelInfo;
		class CRavidDlgModelConfiguration;
		class CRavidBehavior;
		class CRavidDlgModelSearch;

		class AFX_EXT_CLASS CRavidDlgModelManager : public CRavidDialogBase
		{
		public:
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CRavidDlgModelManager);

			DECLARE_DYNAMIC(CRavidDlgModelManager)

			enum EModelSheetTitle
			{
				EModelSheetTitle_Number = 0,
				EModelSheetTitle_Name,
				EModelSheetTitle_Type,
				EModelSheetTitle_Description,
				EModelSheetTitle_Status,
				EModelSheetTitle_Count,
			};

			enum EModelSheetTitleStatus
			{
				EModelSheetTitleStatus_NumberUp = 0,
				EModelSheetTitleStatus_NumberDown,
				EModelSheetTitleStatus_NameUp,
				EModelSheetTitleStatus_NameDown,
				EModelSheetTitleStatus_TypeUp,
				EModelSheetTitleStatus_TypeDown,
				EModelSheetTitleStatus_DescriptionUp,
				EModelSheetTitleStatus_DescriptionDown,
				EModelSheetTitleStatus_StatusUp,
				EModelSheetTitleStatus_StatusDown,
				EModelSheetTitleStatus_Count,
			};


		public:
			CRavidDlgModelManager(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgModelManager();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual bool CreateMe(CWnd* pParent = nullptr);

		// 대화 상자 데이터입니다.
		// #ifdef AFX_DESIGN_TIME
		// 	enum { IDD = IDD_RavidDlgModelManager };
		// #endif

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();
			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnBnClickedBtnRavidDlgModelManagerOpenModel();
			afx_msg void OnBnClickedBtnRavidDlgModelManagerCloseModel();
			afx_msg void OnBnClickedBtnRavidDlgModelManagerDeleteModel();
			afx_msg void OnBnClickedBtnRavidDlgModelManagerCopyModel();

			afx_msg void OnNMTableSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnNMTableBeginEdit(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnNMTableEndEdit(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnNMTableCancelEdit(NMHDR* pNMHDR, LRESULT* pResult);

			afx_msg void OnTreeItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult);
		public:
			bool UpdateModelList();

			bool UpdateConfigureButton();

			void SetDefaltExpandTree(_In_ bool bExpand); // 내부적으로 CreateParameter 될 경우 Expand 여부 결정
			bool GetDefaltExpandTree();
			bool UpdateParameterTree();

			bool ExpandParameterTree(_In_ bool bExpand); // 즉각적인 Expand 여부 결정

			virtual bool AddUndoData();
			virtual bool BackupUndoData(_In_ CRavidBehavior* pBehavior);

			virtual bool ClearUndoData();
			virtual bool ClearRedoData();

			virtual bool RangeUndoData();
			virtual bool RangeRedoData();

			virtual bool Undo();
			virtual bool Redo();

			virtual bool SetParamValue(CString strParameter, CString strValue);

			bool ConvertParameterElementDataToString(_In_ SRavidParameterElement* pRPE, _Out_ CString& strData);

			virtual CString GetParamValue(_In_ CString strParameter);

		protected:
			bool StringTokenize(_In_ CString str, _In_ CString strToken, _Out_ std::vector<CString>* pVct);

			bool IsNumber(_In_ LPCTSTR lpszString);
			bool IsRealNumber(_In_ LPCTSTR lpszString);

			int	GetSelectedModelNumber();
			CModelInfo* GetSelectedModelInfo();

			bool UpdateButtonStatus();

			bool InitTable();

			bool RecalcTable();

			bool OpenModel();

			bool CloseModel();

			bool NewModel();

			bool CopyModel();

			bool DeleteModel();

			bool UpdateParameterSheet();

			bool ClearParameter();

			int GetModelNumber();
			void SetModelNumber(_In_ int nModelNumber);

			bool CreateParameterTree(_In_ std::vector<SRavidParameterElement>* pVctParameterElements, _In_ bool bCheckRowCountExpand);

			bool IsAvailableParameterTree(_In_ std::vector<SRavidParameterElement>* pVctParameterElements);

		private:
			bool CheckModelParamAuthorityField();

		private:
			virtual void OnOK();
			virtual void OnCancel();

		public:
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

			afx_msg void OnBnClickedBtnRavidDlgModelManagerConfigureModelInfo();
			afx_msg void OnBnClickedBtnRavidDlgModelManagerNewModel();
			afx_msg void OnBnClickedBtnRavidDlgModelManagerSearchModel();
			afx_msg void OnBnClickedBtnRavidDlgModelManagerRefreshModel();

			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
			afx_msg void OnClose();

		protected:
			bool m_bIsSheetEditMode = false;
			double m_dblTitleSizeRatio[CRavidDlgModelManager::EModelSheetTitle_Count];
			CString m_strTableBeginData[CRavidDlgModelManager::EModelSheetTitle_Count];
			CRavidTreeView* m_pOTModelParameter = nullptr;
			CRavidDlgModelConfiguration* m_pDlgmodelConfiguration = nullptr;
			CRavidSheet m_tableModelList;
			int m_nModelNumber = -1;

			std::vector<std::pair<CRavidTreeViewItem*, ERavidParameterFieldInputBoxType> > m_vctTreeItem;

			CRect m_crCurrentClient = CRect(0, 0, 0, 0);
			CSize m_szCurrentWindow = CSize(0, 0);

			bool m_bIsOpen = false;

			std::deque<CRavidBehavior*> m_deqUndoBuffer;
			std::deque<CRavidBehavior*> m_deqRedoBuffer;

			CRavidBehavior* m_pBackupUndoData = nullptr;

			CRavidDlgModelSearch* m_pDlgModelSearch = nullptr;

			EModelSheetTitleStatus m_eTitleStatus = EModelSheetTitleStatus_NumberUp;

			bool m_bDefaltExpand = true;
		};
	}
}
