#pragma once
#include "RavidSheet.h"
#include "ModelConfigurationInfo.h"
#include "RavidSheetPropertyCell.h"
#include "RavidTreeView.h"

namespace Ravid
{
	namespace Framework
	{
		class CRavidDlgModelConfigurationSetting;
		class CRavidSheet;

		class CRavidSheetProperty : public CRavidSheet
		{
		public:
			CRavidSheetProperty();
			CRavidSheetProperty(CRavidTreeView* pRTV);
			CRavidSheetProperty(CRavidTreeView* pRTV, CRavidDlgModelConfigurationSetting* pModelSetting);
			~CRavidSheetProperty();
			
			enum EParameterLimit
			{
				EParameterLimit_DecimalCountMax = 20,
				EParameterLimit_PasteCount = 3000,
			};
			bool InitColumnSize();

			bool SetProperty(int nRow, SModelElementProperty* pProperty);
			BOOL SetItemText(int nRow, int nCol, LPCTSTR str);

			void DestroyCell(int nRow, int nCol);

			bool InitSheet(std::vector<SModelElementProperty*>* vctElement);
			bool UpdateSheet(std::vector<SModelElementProperty*>* vctElement);
			bool UpdateSheetData(std::vector<SModelElementProperty*>* vctElement);
			bool UpdateSheetData(int nRow, int nCol, LPCTSTR str);
			bool UpdateTreeData(int nRow, int nCol, LPCTSTR str);
			void ShowSheet();

			virtual void  OnEndEditCell(int nRow, int nCol, CString str);			
			bool CheckEditCell(int nRow, int nCol, CString str, bool bIsCopyString = false);

			bool CheckIntegerConfiguration(CString strUniqueID);
			bool CheckDepth(int nRow, long nDepth);
			bool CheckDepthAvailability(SModelElementProperty *pProperty, long nDepth = 0);
			bool CheckUniqueIDAvailability(int nRow, CString strUniqueID);
			bool CheckDataTypeAvailability(CString strDataType);
			bool CheckDataTypeChange(std::vector<SModelElementProperty*>* vctProperty, long nDepth = 0);
			long CheckDepth(int nRow);
			bool CheckVariableNameAvailability(int nRow, CString &str, bool bIsCopyString = false);
			CString VariableNameAvaility(int nRow, CString str);
			CString CheckDefaultValueAvailability(int nRow, CString str);
			bool CheckInputBoxTypeAvailability(CString strDataType);
			bool CheckDisabledCell(CPoint point);
			bool CheckDisabledCell(int nRow, int nCol);
			bool CheckInputBoxType(ERavidRavidTreeViewInputBoxType eRTIBT);

			bool CheckAuthorityAvailability(CString strAuthority);
			bool CheckAuthorityChildAvailablity(sModelElementProperty *pProperty);
			bool CheckAuthorityParentAvailablity(sModelElementProperty *pProperty);
			
			virtual void EndEditing();
			virtual void CutSelectedText();
			virtual void OnEditPaste();
			virtual BOOL PasteTextToGrid(CRavidSheetCellID cell, COleDataObject* pDataObject, BOOL bSelectPastedCells = TRUE);
			
			void ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText);

			bool SetSheetCellProperty(std::vector<SModelElementProperty*>* vctProperty);
			CString GetPropertyItemText(int nRow, int nCol);	//쓴적없음

			bool DeleteChildClear(std::vector<SModelElementProperty*>* pvctChild);
			bool DeleteParentClear(SModelElementProperty* pProperty);

			bool CheckArrayVariableName(SModelElementProperty* pProperty);

			virtual void OnEditCopy();
			virtual COleDataSource* CopyTextFromGrid();

			std::vector<SModelElementProperty*>* GetPropertySortData();

			CRavidTreeView* GetOptionTreeView();
			void SetOptionTreeView(CRavidTreeView* pTreeView);

			bool SelectedAndFocusCellMove(int nRow, int nCol);

		public:
			DECLARE_MESSAGE_MAP()
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
			afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
			afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
			afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnEndRavidSheetInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult);
		    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
			afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

		public:
			CRavidSheetPropertyCell *m_pSheetPropertyCell = nullptr;

		protected:
			CRavidDlgModelConfigurationSetting * m_pModelSetting = nullptr;
			CRavidTreeView *m_pOptionTree = nullptr;
			std::vector<SModelElementProperty*>* m_pProperty = nullptr;
			int m_nSize = 1;

			bool m_bRowsDelete = false;
			bool m_bPasteEnd = true;
			bool m_bTest = true;

			bool m_bTreeFoundation = false;

			int m_nVScrollPosition = 0;
			int m_nHScrollPosition = 0;

			int m_nSheetColumnSize[EPropertySheetCol_Count] = {0, };
		};
	}
}