#pragma once

#include "RavidObjectEx.h"

#include <vector>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidInputBoxInfo : public CRavidObjectEx
		{
		public:
			CRavidInputBoxInfo();
			CRavidInputBoxInfo(_In_ CString strTitle);
			virtual ~CRavidInputBoxInfo();

			void SetInputBoxTitle(_In_ CString strTitle);
			CString GetInputBoxTitle();

			int GetInputDataInfoCount();

			bool AddInputDataInfo(_In_ LPCTSTR lpszInputDataTitle, _In_opt_ ERavidInputBoxFieldType eType = ERavidInputBoxFieldType_Edit, _In_opt_ LPCTSTR lpszInputData = nullptr, _In_opt_ bool bIsEditBlind = false, LPCTSTR lpszComboList = nullptr);

			bool DeleteInputDataInfo(_In_ CString strInputDataTitle);
			bool DeleteInputDataInfo(_In_ int nIdx);

			bool SetInputDataTitle(_In_ CString strInputDataTitle, _In_ CString strNewInputDataTitle);
			bool SetInputDataTitle(_In_ int nIdx, _In_ CString strNewInputDataTitle);

			CString GetInputDataTitle(_In_ int nIdx);


			bool SetInputFieldType(_In_ CString strInputDataTitle, _In_ ERavidInputBoxFieldType eType);
			bool SetInputFieldType(_In_ int nIdx, _In_ ERavidInputBoxFieldType eType);

			ERavidInputBoxFieldType GetInputFieldType(_In_ CString strInputDataTitle);
			ERavidInputBoxFieldType GetInputFieldType(_In_ int nIdx);


			bool SetInputData(_In_ CString strInputDataTitle, _In_ CString strData);
			bool SetInputData(_In_ int nIdx, _In_ CString strData);

			CString GetInputData(_In_ CString strInputDataTitle);
			CString GetInputData(_In_ int nIdx);

			bool SetInputBlind(_In_ CString strInputDataTitle, _In_ bool bIsBlind);
			bool SetInputBlind(_In_ int nIdx, _In_ bool bIsBlind);

			bool GetInputBlind(_In_ CString strInputDataTitle);
			bool GetInputBlind(_In_ int nIdx);


			bool SetInputComboList(_In_ CString strInputDataTitle, _In_ CString strComboList);
			bool SetInputComboList(_In_ int nIdx, _In_ CString strComboList);

			CString GetInputComboList(_In_ CString strInputDataTitle);
			CString GetInputComboList(_In_ int nIdx);

			bool SetInputHide(_In_ CString strInputDataTitle, _In_ bool bHide);
			bool SetInputHide(_In_ int nIdx, _In_ bool bHide);

			bool GetInputHide(_In_ CString strInputDataTitle);
			bool GetInputHide(_In_ int nIdx);

			void SetComboDataTypeIndex(_In_ bool bSet);
			bool IsComboDataTypeIndex();

			void SetComboEditEnable(_In_ bool bEnable);
			bool IsComboEditEnable();

			void SetComboInitializeSel(bool bSet);
			bool IsComboInitializeSel();

			bool AddCondition(_In_ CString strInputDataTitle, _In_ CString strCheckData, _In_ ELogicalCondition eCondition, _In_ CString strTargetInputDataTitle, _In_opt_ bool* pBHide = nullptr, _In_opt_ ERavidInputBoxFieldType* pEChangeType = nullptr, _In_opt_ CString* pStrChangeTitle = nullptr, _In_opt_ CString* pStrChangeData = nullptr, _In_opt_ CString* pStrChangeComboList = nullptr, _In_opt_ bool* pBChangeBlindText = nullptr);

			bool AddCondition(_In_ int nIdx, _In_ CString strCheckData, _In_ ELogicalCondition eCondition, _In_ int nTargetIdx, _In_opt_ bool* pBHide = nullptr, _In_opt_ ERavidInputBoxFieldType* pEChangeType = nullptr, _In_opt_ CString* pStrChangeTitle = nullptr, _In_opt_ CString* pStrChangeData = nullptr, _In_opt_ CString* pStrChangeComboList = nullptr, _In_opt_ bool* pBChangeBlindText = nullptr);

			std::vector<SRavidInputConditionInfo>* GetConditionInfos();

		protected:
			bool m_bIsComboDataTypeIndex = true;
			bool m_bIsComboEditEnable = false;
			bool m_bIsComboInitializeSel = true;

			CString m_strInputBoxTitle = _T("");

			std::vector<SRavidInputDlgDataInfo> m_vctInputDataInfos;
			std::vector<SRavidInputConditionInfo> m_vctConditionInfo;
		};
	}
}