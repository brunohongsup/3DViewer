#pragma once


// CRavidDlgModelConfigurationProperty 대화 상자입니다.

#include "RavidDialogBase.h"
#include "RavidSheet.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidDlgModelConfigurationProperty : public CRavidDialogBase
		{
			RavidPreventCopySelf(CRavidDlgModelConfigurationProperty);

		public:
			DECLARE_DYNAMIC(CRavidDlgModelConfigurationProperty)

		public:
			CRavidDlgModelConfigurationProperty(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgModelConfigurationProperty();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual bool CreateMe(CWnd* pParent = nullptr);

		// 대화 상자 데이터입니다.
		// #ifdef AFX_DESIGN_TIME
		// 	enum { IDD = IDD_RAVID_DLG_MODEL_CONFIGURATION_PROPERTY };
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

			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationPropertyOk();
			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationPropertyCancel();

		public:
			bool SetModelElementProperty(_In_ SModelElementProperty* pMEP);

		protected:
			bool InitSheet();
			bool UpdateSheet();


			bool CheckUniqueIDAvailability(_In_ unsigned long ulUniqueID);
			bool CheckDataTypeAvailability(_In_ ERavidParameterDataType eRPDT);
			bool CheckVariableNameAvailability(_In_ CString strVariableName);
			bool CheckInputBoxTypeAvailability(_In_ ERavidRavidTreeViewInputBoxType eRPIBT);

			DWORD CheckElementPropertyAvailability();

			bool SetMessage(_In_ CString strMessage);

		protected:
			enum EPropertySheetRow
			{
				EPropertySheetRow_UniqueID = 0,
				EPropertySheetRow_Name,
				EPropertySheetRow_Description,
				EPropertySheetRow_DataType,
				EPropertySheetRow_VariableName,
				EPropertySheetRow_DefaultValue,
				EPropertySheetRow_InputBoxType,
				EPropertySheetRow_ExtraData,
				EPropertySheetRow_DecimalCount,
				EPropertySheetRow_Count,
			};

			enum EElementPropertyAvailabilityResult
			{
				EElementPropertyAvailabilityResult_OK = 0x00,
				EElementPropertyAvailabilityResult_UnknownError = 0x01,
				EElementPropertyAvailabilityResult_UniqueIDError = 0x02,
				EElementPropertyAvailabilityResult_DataTypeError = 0x04,
				EElementPropertyAvailabilityResult_VariableNameError = 0x08,
				EElementPropertyAvailabilityResult_InputBoxTypeError = 0x10,
			};

			bool m_bIsSheetEditMode = false;

			CRavidSheet m_sheetProperty;

			SModelElementProperty* m_pMEPOrg = nullptr;
			SModelElementProperty m_mepCopy;
			
		};
	}
}
