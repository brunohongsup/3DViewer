#pragma once

#include "RavidDialogBase.h"

namespace Ravid
{
	namespace Framework
	{

		class CRavidSheet;

		class CRavidDlgModelConfigurationPropertyArray : public CRavidDialogBase
		{
		public:
			DECLARE_DYNAMIC(CRavidDlgModelConfigurationPropertyArray)

			CRavidDlgModelConfigurationPropertyArray(CWnd* pParent = NULL);
			CRavidDlgModelConfigurationPropertyArray(CWnd* pParent, SModelElementProperty* pProperty);
			virtual ~CRavidDlgModelConfigurationPropertyArray();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual BOOL OnInitDialog();

			bool SetMessage(_In_ CString strMessage);
			bool SetModelElementProperty(SModelElementProperty *pProperty);

			bool DevideVariable(_Inout_ CString& strVariable, _Out_ long& nArraySize);
			void RegisterModelConfigurationInfo(_In_ CModelConfigurationInfo* pRMCI);
			CModelConfigurationInfo* GetModelConfigurationInfo();

		public:
			bool InitSheet();
			bool SaveArrayVariableName();

		protected:
			CRavidSheet *m_pSheet = nullptr;
			CString m_strArraySize = _T("");
			std::vector<CString> m_vctArrayName;
			SModelElementProperty *m_pMEPOrg = nullptr;
			CModelConfigurationInfo* m_pMCI = nullptr;

		public:
			afx_msg void OnDestroy();
			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationPropertyArrayOk();
			afx_msg void OnBnClickedBtnRavidDlgModelConfigurationPropertyArrayCancel();
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		};
	}
}


