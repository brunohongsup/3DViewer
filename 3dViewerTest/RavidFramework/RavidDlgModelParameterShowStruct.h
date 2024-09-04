#pragma once


// CRavidDlgModelParameterShowStruct 대화 상자입니다.

#include "RavidDialogBase.h"

#include <memory>

namespace Ravid
{
	namespace Framework
	{
		class CModelConfigurationInfo;

		class AFX_EXT_CLASS CRavidDlgModelParameterShowStruct : public CRavidDialogBase
		{

		public:
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CRavidDlgModelParameterShowStruct);
			DECLARE_DYNAMIC(CRavidDlgModelParameterShowStruct)

		public:
			CRavidDlgModelParameterShowStruct(CWnd* pParent = NULL);   // 표준 생성자입니다.
			virtual ~CRavidDlgModelParameterShowStruct();

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

		public:
			void RegisterModelConfigurationInfo(_In_ CModelConfigurationInfo* pRMCI);

		protected:
			bool Show();

		protected:
			static LPCTSTR m_lpszModelParameterDataType[ERavidParameterDataType_Count];
			CModelConfigurationInfo* m_pRMCI = nullptr;

		};
	}
}
