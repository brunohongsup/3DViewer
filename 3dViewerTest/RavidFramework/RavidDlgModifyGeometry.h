#pragma once

#include "RavidDialogBase.h"
#include "RavidSheet.h"

#include <memory>

namespace Ravid
{
	namespace Framework
	{
		class CViewObjectElement;
		class AFX_EXT_CLASS CRavidDlgModifyGeometry : public CRavidDialogBase
		{
			enum ERavidModifyGeometryComboType
			{
				ERavidModifyGeometryComboType_Offset = 0,
				ERavidModifyGeometryComboType_Shape,
				ERavidModifyGeometryComboType_Count,
			};

			enum ERavidModifyGeometrySheetPosition
			{
				ERavidModifyGeometrySheetPosition_Parameter = 0,
				ERavidModifyGeometrySheetPosition_Value,
				ERavidModifyGeometrySheetPosition_Count,
			};


			enum ERavidModifyGeometryOffsetRow
			{
				ERavidModifyGeometryOffsetRow_X = 1,
				ERavidModifyGeometryOffsetRow_Y,
			};

			enum ERavidModifyGeometryPointRow
			{
				ERavidModifyGeometryPointRow_PointX = 1,
				ERavidModifyGeometryPointRow_PointY,
			};

			enum ERavidModifyGeometryLineRow
			{
				ERavidModifyGeometryLineRow_Point1X = 1,
				ERavidModifyGeometryLineRow_Point1Y,
				ERavidModifyGeometryLineRow_Point2X,
				ERavidModifyGeometryLineRow_Point2Y,
			};

			enum ERavidModifyGeometryRectangleRow
			{
				ERavidModifyGeometryRectangleRow_Left = 1,
				ERavidModifyGeometryRectangleRow_Top,
				ERavidModifyGeometryRectangleRow_Right,
				ERavidModifyGeometryRectangleRow_Bottom,
				ERavidModifyGeometryRectangleRow_Angle,
			};

			enum ERavidModifyGeometryQuadrangleRow
			{
				ERavidModifyGeometryQuadrangleRow_Point1X = 1,
				ERavidModifyGeometryQuadrangleRow_Point1Y,
				ERavidModifyGeometryQuadrangleRow_Point2X,
				ERavidModifyGeometryQuadrangleRow_Point2Y,
				ERavidModifyGeometryQuadrangleRow_Point3X,
				ERavidModifyGeometryQuadrangleRow_Point3Y,
				ERavidModifyGeometryQuadrangleRow_Point4X,
				ERavidModifyGeometryQuadrangleRow_Point4Y,
			};

			enum ERavidModifyGeometryCircleRow
			{
				ERavidModifyGeometryCircleRow_CenterX = 1,
				ERavidModifyGeometryCircleRow_CenterY,
				ERavidModifyGeometryCircleRow_Radius,
			};

			enum ERavidModifyGoemetryEllipseRow
			{
				ERavidModifyGeometryEllipseRow_CenterX = 1,
				ERavidModifyGeometryEllipseRow_CenterY,
				ERavidModifyGeometryEllipseRow_Radius1,
				ERavidModifyGeometryEllipseRow_Radius2,
				ERavidModifyGeometryEllipseRow_Angle,
			};

		public:
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CRavidDlgModifyGeometry);
			DECLARE_DYNAMIC(CRavidDlgModifyGeometry)


		public:
			CRavidDlgModifyGeometry(CWnd* pParent = nullptr);   // 표준 생성자입니다.
			virtual ~CRavidDlgModifyGeometry();

			virtual BOOL OnInitDialog();
			afx_msg void OnClose();
			virtual bool Create(_In_opt_ CWnd* pParent = nullptr);

			afx_msg void OnSheetEndEdit(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnSheetCancelEdit(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnSheetBeginEdit(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnCbnSelendokComboRavidDlgModifyGeometryList();

			afx_msg void OnBnClickedBtnRavidDlgModifyGeometryApply();
			afx_msg void OnBnClickedBtnRavidDlgModifyGeometryCancel();
			afx_msg void OnBnClickedBtnRavidDlgModifyGeometryPreview();
			virtual BOOL PreTranslateMessage(MSG* pMsg);

			bool SheetStringCheck(_In_ bool bMinus = true);
			bool InitSheet();
			bool UpdateSheet(_In_ bool bReset = true);

			bool SetViewObjectElement(_In_ CViewObjectElement* pVOE);
			CViewObjectElement* GetViewObjectElement();

			bool GetPreviewGeometry(_Out_ CRavidGeometry * pRG);

			bool ResetPrameterValues();

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);
			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			DECLARE_MESSAGE_MAP()

		protected:
			CComboBox m_ctrlComboDropList;

			CRavidSheet m_sheetProperty;

			CViewObjectElement* m_pViewObjectElement = nullptr;

			CString m_strOffsetX;
			CString m_strOffsetY;
			CRavidGeometry * m_pPreviewGeometry = nullptr;

			bool m_bIsSheetEditMode = false;

		};

	}
}


