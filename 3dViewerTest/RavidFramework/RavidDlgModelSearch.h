#pragma once

#include "RavidDialogBase.h"

namespace Ravid
{

	namespace Framework
	{
		class CModelInfo;
		class AFX_EXT_CLASS CRavidDlgModelSearch : public CRavidDialogBase
		{

		public:
			DECLARE_DYNAMIC(CRavidDlgModelSearch)

		public:
			CRavidDlgModelSearch(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgModelSearch();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual bool CreateMe(CWnd* pParent = nullptr);

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		public:
			virtual BOOL OnInitDialog();

			afx_msg void OnBnClickedOk();
			afx_msg void OnBnClickedCancel();

			afx_msg void OnEnChangeEditSearch();

			afx_msg void OnCbnSelchangeComboSearch();

			afx_msg void OnClose();

		public:
			virtual bool UpdateModelSearchInfo();

		protected:
			CComboBox m_comboSearch;

			CEdit m_editSearch;

		public:
			std::vector<CModelInfo*> m_vctModelSearchInfo;
		};
	}
}
