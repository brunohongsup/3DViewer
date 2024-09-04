#pragma once

#include "RavidObjectEx.h"

#include <vector>
#include <memory>

namespace Ravid
{
	namespace Database
	{
		class CRavidDatabase;
		class CRavidRecordset;
	}

	namespace Framework
	{
		class CModelInfo;
		class CModelConfigurationInfo;
		
		/**
		* 프레임워크 모델을 관리하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CModelManager : public CRavidObjectEx
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CModelManager);

		private:
			CModelManager();

		public:
			virtual ~CModelManager();
			
			// 초기화
			static bool Initialize(_In_ LPCTSTR lpszModelRootPath, _In_opt_ LPCTSTR lpszParameterFileName = _T("ModelParam.mvtdb"));

			// 초기화 해제
			static bool Terminate();

			// 초기화 상태 확인
			static bool IsInitialized();

			static CString GetParameterFileName();

			// 모델 Root 경로 가져오기
			static CString GetModelRootPath();

			// 모델 리스트 갱신
			static bool UpdateModelList();


			// Model Info 인스턴스의 전체 개수를 구한다.
			static size_t GetModelInfoCount();

			// 해당 index의 ModelInfo 인스턴스를 얻어온다.
			static CModelInfo* GetModelInfo(_In_ int nNumber);


			// 해당 모델 번호에 해당하는 Model Info를 찾는다.
			static CModelInfo* FindModelInfo(_In_ int nModelNo);

			// 해당 모델 이름에 해당하는 Model Info를 찾는다.
			static bool FindModelInfo(_In_ LPCTSTR lpszModelName, _Out_ std::vector<CModelInfo*>* pVctResult);

			// 해당 모델 번호에 해당하는 Model Info를 연다. 만약 같은 번호가 존재할 시 해당 번호와 같은 모델을 전부 연다.
			static bool OpenModelInfo(_In_ int nModelNo);

			// 해당 모델 이름에 해당하는 Model Info를 연다. 만약 같은 이름이 존재할 시 해당 이름과 같은 모델을 전부 연다.
			static bool OpenModelInfo(_In_ LPCTSTR lpszModelName);


			// 해당 모델 번호에 해당하는 Model Info를 닫는다. 만약 같은 번호가 존재할 시 해당 번호와 같은 모델을 전부 닫는다.
			static bool CloseModelInfo(_In_ int nModelNo);

			// 해당 모델 이름에 해당하는 Model Info를 닫는다. 만약 같은 이름이 존재할 시 해당 이름과 같은 모델을 전부 닫는다.
			static bool CloseModelInfo(_In_ LPCTSTR lpszModelName);


			// 해당 모델 번호에 해당하는 Model Info를 삭제한다. 만약 같은 번호가 존재할 시 해당 번호와 같은 모델을 전부 삭제한다.
			static bool DeleteModelInfo(_In_ int nModelNo);

			// 해당 모델 이름에 해당하는 Model Info를 삭제한다. 만약 같은 이름이 존재할 시 해당 이름과 같은 모델을 전부 삭제한다.
			static bool DeleteModelInfo(_In_ LPCTSTR lpszModelName);


			// 해당 모델 번호에 해당하는 Model Info를 복사한다. 만약 같은 번호가 존재할 시 해당 번호와 같은 모델을 전부 복사한다.
			static bool CopyModelInfo(_In_ int nModelNo);

			// 해당 모델 이름에 해당하는 Model Info를 복사한다. 만약 같은 이름이 존재할 시 해당 이름과 같은 모델을 전부 복사한다.
			static bool CopyModelInfo(_In_ LPCTSTR lpszModelName);

			static bool NewModelInfo(_In_ LPCTSTR lpszModelTypeName);


			static CString GetModelParameterDataTypeString(_In_ ERavidParameterDataType eRPDT);

			static CString GetModelParameterAuthorityString(_In_ EUserType eType);

			static SModelTypeInfo GetModelTypeInfo(_In_ CString strModelTypeName);

			static std::vector<SModelTypeInfo> GetModelTypeInfoList();

			static long GetModelTypeInfoSequenceNumber(_In_ CString strModelTypeName);

			static bool IsBeingSameModelTypeName(_In_ CString strModelTypeName);

			static bool RearrangementModelTypeInfo();

			static bool IsOpenedModelType(_In_ CString strModelTypeName);
			static bool IsUsedModelType(_In_ CString strModelTypeName);


			static bool NewModelType(_In_ CModelConfigurationInfo* pRMCI);

			static bool ModifyModelType(_In_ CString strOrgModelTypeName, _In_ CModelConfigurationInfo* pRMCI);

			static bool DeleteModelType(_In_ CString strModelTypeName);

			static bool CopyModelType(_In_ CModelConfigurationInfo* pRMCI);

			static bool IsAbleToOpen(_In_ int nModelNo);

			static bool ChangeModelNumber(_In_ int nCurrentModelNo, _In_ int nNewModelNo);
			static bool ChangeModelName(_In_ int nCurrentModelNo, _In_ CString strNewModelName);
		private:

			// 모델 리스트 클리어
			static bool ClearModelList();

			static CModelManager* GetInstance();

			static bool OpenTable(_In_ Database::CRavidDatabase* pDatabase, _In_ CString strTableName, _Out_ Database::CRavidRecordset* pRS);

			static bool CreateModelTypeInfoTable();

			static bool OpenModelInfo(_In_ CModelInfo* pModelInfo);

		private:
			static LPCTSTR m_lpszModelParameterDataType[ERavidParameterDataType_Count];
			static LPCTSTR m_lpszModelTypeInfoTableFieldTitle[EModelTypeInfoTableField_Count];
			static LPCTSTR m_lpszModelParameterAuthority[EUserType_Count];

		protected:
			bool m_bInitialized = false;
			CString m_strModelRootPath;
			CString m_strParameterFileName;
			std::vector<CModelInfo*> m_vctModelInfo;

			const CString m_strModelTypeInfoTableName = _T("Model type info");
		};
	}
}

