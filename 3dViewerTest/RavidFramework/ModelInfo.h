#pragma once

#include "RavidObjectEx.h"

#include <vector>
#include <memory>

namespace Ravid
{
	namespace Database
	{
		class CRavidDatabase;
	}

	namespace Framework
	{
		class CModelConfigurationInfo;

		class AFX_EXT_CLASS CModelInfo : public CRavidObjectEx
		{
		public:
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CModelInfo);

			CModelInfo();
			virtual ~CModelInfo();

			bool Initialize(_In_ LPCTSTR lpszModelFile);
			bool Terminate();

			bool IsInitialized();

			int GetModelNumber();
			CString GetModelName();

			void SetModelNumber(_In_ int nNumber);
			void SetModelName(_In_ CString strName);

			CString GetModelFileFullPath();

			bool Open();
			bool IsOpen();
			bool Close();

			bool SetModelDescription(_In_ CString strDescription);

			CString GetModelDescription();
			CString GetModelType();

			bool GetData(_Out_ void* pModelStruct, _In_ size_t szStructureSize);
			bool SaveData(_In_ void* pModelStruct, _In_ size_t szStructureSize);

			std::vector<SRavidParameterElement>* GetParameterElements();

			bool ModifyModel(_In_ CModelConfigurationInfo* pRMCI);

			bool UpdateToDatabase();
			bool UpdateToDatabase(int nParameterIndex);

			Database::CRavidDatabase* GetDataBase();

		private:
			bool NewModelInfo(LPCTSTR lpszModelFile, LPCTSTR lpszModelType);

			bool CreateDatabase(LPCTSTR lpszModelFile, CModelConfigurationInfo * pRMCI);

			bool CheckModelParamAuthorityField();

			friend class CModelManager;
			friend class CRavidDlgModelManager;

		protected:
			enum EModelTable
			{
				EModelTable_Parameter = 0,
				EModelTable_Information,
				EModelTable_Count,
			};

			enum EInformationTableField
			{
				EInformationTableField_ModelType = 0,
				EInformationTableField_Description,
				EInformationTableField_Count,
			};

			bool m_bInitialized = false;

			int m_nModelNumber = -1;
			CString m_strModelName;
			CString m_strModelFileFullPath;

			Database::CRavidDatabase* m_pDatabase = nullptr;

			static const char* m_lpszDescriptionHeader;
			static LPCTSTR m_lpszModelTable[EModelTable_Count];
			static LPCTSTR m_lpszModelParameterTableField[ERavidParameterFieldName_Count];
			static LPCTSTR m_lpszModelInformationTableField[EInformationTableField_Count];

			std::vector<SRavidParameterElement> m_vctParameterElement;
			SRavidparameterInformation m_sParameterInformation;
		};
	}
}
