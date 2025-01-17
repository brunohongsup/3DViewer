#pragma once

#include "RavidDialogBase.h"

// CRavidDlgOperationGeometry 대화 상자

namespace Ravid
{
	namespace Framework
	{
		class CViewObjectElement;
		class CViewObject;

		class CRavidDlgOperationGeometry : public CRavidDialogBase
		{
			DECLARE_DYNAMIC(CRavidDlgOperationGeometry)

		public:
			CRavidDlgOperationGeometry(CWnd* pParent = nullptr);   // 표준 생성자입니다.
			virtual ~CRavidDlgOperationGeometry();

		// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_RAVID_IMAGE_TOOL_OPERATION };
#endif

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			DECLARE_MESSAGE_MAP()

		public:
			virtual bool SetGeometryItem(_In_ std::vector<CViewObjectElement*> vctViewObjectElement, _In_ std::vector<CString> vctGeometryTitle);
			virtual bool SetCurrentViewObjectElement(_In_ CViewObjectElement* pViewObjectElement);
			virtual bool SetSelectdViewObjectElement(_In_ CViewObjectElement* pViewObjectElement);
			virtual void SetGeometryItemIndex(_In_ int nIndex);
			virtual void SetTitle(_In_ CString strTitle);

			virtual bool ClearGeometryItem();

			virtual void CloseOperationGeometry();

			afx_msg void OnClose();
			virtual bool Create(_In_opt_ CWnd* pParent = nullptr);
			virtual BOOL OnInitDialog();
			virtual void PostNcDestroy();

			afx_msg void OnBnClickedOk();
			afx_msg void OnBnClickedCancel();
			afx_msg void OnCbnSelchangeComboGeometry();

		private:
			CViewObjectElement* m_pCurrentViewObjectElement = nullptr;
			CViewObjectElement* m_pSelectedViewObjectElement = nullptr;

			int m_nComboboxSelectedIdx = 0;
			CString m_strTitle = _T("");

		};
	}
}
