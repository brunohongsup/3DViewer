#pragma once

#include "RavidObjectEx.h"

#include <vector>
#include <memory>
#include <set>

namespace Ravid
{
	namespace Database
	{
		class CRavidDatabase;
		class CRavidRecordset;
	}

	namespace Framework
	{
		class AFX_EXT_CLASS CModelConfigurationInfo : public CRavidObjectEx
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CModelConfigurationInfo);

		public:
			CModelConfigurationInfo();
			virtual ~CModelConfigurationInfo();

			bool Initialize(_In_ LPCTSTR lpszModelTypeName);
			bool Terminate();

			bool IsInitialized();

			void SetModelTypeSequenceNumber(_In_ long nSequence);
			long GetModelTypeSequenceNumber();

			void SetModelTypeName(_In_ CString strModelTypeName);
			CString GetModelTypeName();

			void SetModelTypeDescription(_In_ CString strModelTypeDescription);
			CString GetModelTypeDescription();

			void SetModelTypeConcurrentSize(_In_ long nSize);
			long GetModelTypeConcurrentSize();

			long GetElementPropertyCount();

			std::vector<SModelElementProperty*>* GetElementProperties();

			std::vector<SModelElementProperty*> GetSequenceOrderElementProperties();

			bool AddElementProperty(_In_ SModelElementProperty* pRMEP, _In_opt_ bool bLocationUp = false, _In_opt_ SModelElementProperty* pLocationRMEP = nullptr);
			bool DeleteElementProperty(_In_ SModelElementProperty* pRMEP);

			bool CopyElementProperty(_In_ SModelElementProperty* pSrcMEP, _Out_ SModelElementProperty** pDstMEP);

			std::set<unsigned long> GetElementPropertyAllUniqueID();
			std::vector<CString> GetElementPropertyAllVariableName();

			unsigned long GetElementPropertyEmptyUniqueID();

			CString GetElementPropertyEmptyVariableName(CString strVariableName);
			int m_nVariableNameSetGarbageCount = 0;

			bool IsEmptyElementPropertyUniqueID(_In_ unsigned long ulUniqueID);

			bool IsBeingSameElementPropertyVariableName(_In_ CString strVariableName);

			bool ElementPropertyDefaultSetting(_In_ SModelElementProperty* pRMEP);

			SModelElementProperty* GetParentElementProperty(_In_ SModelElementProperty* pMEP);
			SModelElementProperty* GetPrevElementProperty(_In_ SModelElementProperty* pMEP);
			SModelElementProperty* GetNextElementProperty(_In_ SModelElementProperty* pMEP);

			bool CreateModelConfigurationTable();

			bool DeleteModelConfigurationTable();

			bool CheckModelTypeNameAvailability();

			bool CheckVariableNameAvailability(_In_ CString strVariableName);

			bool DevideVariable(_Inout_ CString& strVariable, _Out_ long& nArraySize);

		private:
			bool OpenTable(_In_ Database::CRavidDatabase* pDatabase, _In_ CString strTableName, _Out_ Database::CRavidRecordset* pRS);

			bool IsTreeOrder();

			bool ConvertTreeOrderToSequenceOrder();
			bool ConvertSequenceOrderToTreeOrder();

		protected:
			static LPCTSTR m_lpszModelConfigurationTableFieldTitle[EModelConfigurationTableField_Count];
			static LPCTSTR m_lpszModelConfigurationTableFieldTitleVariableArrayType[EModelConfigurationTableFieldVariableArrayType_Count];

			const CString m_strPrefixModelConfigurationTableName = _T("Model configuration");
			const CString m_strPrefixModelConfigurationArrayTypeTableName = _T("Model configuration Array type");

			bool m_bInitialized = false;

			bool m_bIsTreeOrder = false;

			SModelTypeInfo m_sRMTN;

			std::vector<SModelElementProperty*> m_vctRMEP;

		};
	}
}

