#pragma once

#include <vector>
#include <afxmt.h>

namespace Ravid
{
	namespace Miscellaneous
	{
		class CSimpleEncryptor;
	}

	namespace Database
	{
		class CInternalRavidDatabase;
		class CRavidRecordset;
		class CRavidRecordsetFieldInformation;
		
		/**
		* 데이터 베이스를 관리하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CRavidDatabase
		{
		public:
			RavidPreventCopySelf(CRavidDatabase);

			CRavidDatabase();
			virtual ~CRavidDatabase();
			
			/**
			* 데이터베이스 파일을 엽니다.
			*
			* param[in] strFilePath : 파일명
			* param[in_opt] strPassword : 비밀번호
			* param[in_opt] bReadOnly : 읽기 전용 여부
			* param[in_opt] bCreateIfNoExistence : 파일이 없을 시 생성 여부
			*
			* return value : 실패하면 ERavidDatabaseResult_OK 이외의 값을 가집니다.
			*/
			ERavidDatabaseResult Open(_In_ CString strFilePath, _In_opt_ CString strPassword = _T(""), _In_opt_ bool bReadOnly = false, _In_opt_ bool bCreateIfNoExistence = true);
			
			/**
			* 현재 열린 데이터베이스 파일을 저장합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Save();
			
			/**
			* 현재 열린 데이터베이스 파일을 닫습니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Close();
			
			/**
			* 데이터베이스 파일을 생성합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Create();
			
			/**
			* 데이터베이스 파일의 열린 상태를 확인합니다.
			*
			* return value : bool 의 열린 상태를 반환합니다.
			*/
			bool IsOpen();
			
			/**
			* 데이터베이스 파일의 읽기 전용 상태를 확인합니다.
			*
			* return value : bool 의 읽기 전용 상태를 반환합니다.
			*/
			bool IsReadOnly();
			
			/**
			* 데이터베이스에 쿼리문을 실행합니다.
			*
			* param[in] strSQL : SQL 쿼리문
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool ExecuteSQL(_In_ CString strSQL);
			
			/**
			* 데이터베이스에 등록된 테이블 수를 가져옵니다.
			*
			* return value : size_t 의 테이블 수를 반환합니다.
			*/
			size_t GetTableCount();
						
			/**
			* 데이터베이스에 등록된 테이블 리스트를 가져옵니다.
			*
			* param[in] pVctTableNameList : 테이블 리스트 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetTableNameList(_Out_ std::vector<CString>* pVctTableNameList);
			
			/**
			* 데이터베이스에 해당 테이블 유무를 확인합니다.
			*
			* param[in] strTableName : 테이블명
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool DoesTableExist(_In_ CString strTableName);
			
			/**
			* 데이터베이스에 해당 테이블에 해당 필드 유무를 확인합니다.
			*
			* param[in] strTableName : 테이블명
			* param[in] strFieldName : 필드명
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool DoesFieldExist(_In_ CString strTableName, _In_ CString strFieldName);
			
			/**
			* 데이터베이스에 비밀번호를 설정합니다.
			*
			* param[in] strCurrentPassword : 기존 비밀번호
			* param[in] strNewPassword : 신규 비밀번호
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetPassword(_In_ CString strCurrentPassword, _In_ CString strNewPassword);
			
			/**
			* 데이터베이스의 파일명을 가져옵니다.
			*
			* return value : CString 의 파일명을 반환합니다.
			*/
			CString GetDBFilePath();
			
			/**
			* 데이터베이스에 해당 테이블을 생성합니다.
			*
			* param[in] strTableName : 테이블명
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			CRavidRecordset* CreateTable(_In_ CString strTableName);
			
			/**
			* 데이터베이스에 해당 인덱스의 테이블을 삭제합니다.
			*
			* param[in] nIndex : 테이블 인덱스
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool DropTable(_In_ long nIndex);

			/**
			* 데이터베이스에 해당 테이블을 삭제합니다.
			*
			* param[in] strTableName : 테이블명
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool DropTable(_In_ CString strTableName);
			
			/**
			* 데이터베이스에 해당 인덱스의 테이블 필드 정보를 가져옵니다.
			*
			* param[in] nIndex : 테이블 인덱스
			*
			* return value : CRavidRecordset* 의 레코드 정보를 반환합니다.
			*/
			CRavidRecordset* GetTable(_In_ long nIndex);
			
			/**
			* 데이터베이스에 해당 테이블의 정보를 가져옵니다.
			*
			* param[in] nIndex : 테이블 인덱스
			*
			* return value : CRavidRecordset* 의 레코드 정보를 반환합니다.
			*/
			CRavidRecordset* GetTable(_In_ CString strTableName);
			
			/**
			* 데이터베이스에 해당 인덱스의 테이블에 필드를 설정합니다.
			*
			* param[in] nIndex : 테이블 인덱스
			* param[in] pRecordset : 테이블 필드 정보 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetTable(_In_ long nIndex, _In_ CRavidRecordset* pRecordset);
			
			/**
			* 데이터베이스에 해당 테이블에 필드를 설정합니다.
			*
			* param[in] strTableName : 테이블명
			* param[in] pRecordset : 테이블 필드 정보 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetTable(_In_ CString strTableName, _In_ CRavidRecordset* pRecordset);
			
			/**
			* 데이터베이스에 해당 인덱스의 테이블명을 가져옵니다.
			*
			* param[in] nIndex : 테이블 인덱스
			*
			* return value : CString 의 테이블명을 반환합니다.
			*/
			CString GetTableNameOfIndex(_In_ long nIndex);
			
			/**
			* 데이터베이스에 해당 테이블명의 인덱스를 가져옵니다.
			*
			* param[in] strTableName : 테이블명
			*
			* return value : long 의 테이블 인덱스를 반환합니다.
			*/
			long GetIndexOfTableName(_In_ CString strTableName);
			
			/**
			* 데이터베이스에 해당 인덱스의 테이블 명을 변경합니다.
			*
			* param[in] nIndex : 테이블 인덱스
			* param[in] strNewTableName : 신규 테이블 명
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool ChangeTableName(_In_ long nIndex, _In_ CString strNewTableName);
			
			/**
			* 데이터베이스에 해당 테이블 명을 변경합니다.
			*
			* param[in] strTableName : 기존 테이블 명
			* param[in] strNewTableName : 신규 테이블 명
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool ChangeTableName(_In_ CString strTableName, _In_ CString strNewTableName);

		private:
			bool ConvertDataTypeStringToEnum(_In_ CString strDataType, _Out_ ERavidRecordsetFieldType& eDataType);
			bool ConvertStringOperatorToEnum(_In_ CString strOperator, _Out_ ERavidDBWhereClauseOperator& eOperator);

			bool ParseWhereClause(_In_ CString strWhere, _In_ std::vector<CRavidRecordsetFieldInformation*>& vctFields, _Out_ std::vector<SRavidDBWhereClauseCondition>& vctWhereCondition);

			bool ExtractString(_In_ CString& strSource, _In_ TCHAR chDelimiter, _Out_ CString& strResult);
			bool ExtractString(_In_ CString& strSource, _In_ std::vector<TCHAR>& vctDelimiter, _Out_ CString& strResult);

			bool DoesTableExist_Internal(_In_ CString& strTableName);
			bool Close_Internal();

			bool LockDBFile();
			bool UnlockDBFile();

			ERavidDatabaseResult ReadDBFile();
			bool WriteDBFile();

			bool ReadFile(_Out_ void* pDstBuffer, _In_ size_t szElementSize, _In_ FILE* pFilePointer, _In_opt_ bool bDecrypt = true);
			bool WriteFile(_In_ const void* pSrcBuffer, _In_ size_t szElementSize, _In_ FILE* pFilePointer, _In_opt_ bool bEncrypt = true);

			bool AssignBuffer(_Inout_ char** pBuffer, size_t szSize);
			bool DeleteBuffer(_Inout_ char** pBuffer);

			bool IsPasswordEmpty();

			void SetDBFilePath(_In_ CString strDBFilePath);
			/*CString GetDBFilePath();*/

			bool ExtractValue(_In_ CString& strSource, _In_ std::vector<CString>& vctDelimiter, _In_ std::vector<CString>& vctEnd, _Out_ CString& strResult, _Out_ bool& bEnd);

			bool EncryptData(_In_ CString strData, _Out_ std::vector<char>& vctData);
			bool DecryptData(_In_ std::vector<char>& vctData, _Out_ CString& strData);

			CInternalRavidDatabase* m_pInternal = nullptr;
			
			static const char m_arrDBFileHeader[];
			static const ptrdiff_t m_nDBFileHeaderLength;

			friend class CRavidRecordset;

			friend class CDBEditorManager;
		};
	}
}

