#pragma once

#include <memory>

namespace Ravid
{
	namespace Database
	{
		class CRavidDatabase;
		class CRavidRecordsetFieldInformation;
		class CInternalRavidRecordset;
		
		/**
		* 데이터 베이스의 필드를 관리하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CRavidRecordset
		{
		public:
			RavidPreventCopySelf(CRavidRecordset);

			/**
			* 데이터 베이스의 필드 타입을 가집니다.
			*/
			static const TCHAR* m_arrFieldTypeString[];

			CRavidRecordset();
			CRavidRecordset(_In_ CRavidDatabase* pParentDataStorage, _In_ CString strSQL);

			virtual ~CRavidRecordset();
			
			/**
			* 입력 데이터베이스에 해당 쿼리문을 실행합니다.
			*
			* param[in] pParentDataStorage : 데이터베이스 클래스 (Pointer)
			* param[in] strSQL : 쿼리문
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Open(_In_ CRavidDatabase* pParentDataStorage, _In_ CString strSQL);

			/**
			* 데이터베이스 객체를 닫습니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Close();
			
			/**
			* 데이터베이스 객체의 열린 상태를 확인합니다.
			*
			* return value : bool 의 열린 상태를 반환합니다.
			*/
			bool IsOpen();
			
			/**
			* 현재 객체의 테이블 명을 가져옵니다.
			*
			* return value : CString 의 테이블 명을 반환합니다.
			*/
			CString GetTableName();
			
			/**
			* 현재 객체의 필드 수를 가져옵니다.
			*
			* return value : size_t 의 필드 수를 반환합니다.
			*/
			size_t GetFieldCount();
			
			/**
			* 현재 객체의 레코드(데이터) 수를 가져옵니다.
			*
			* return value : size_t 의 필드 수를 반환합니다.
			*/
			size_t GetRecordCount();
			
			/**
			* 입력 필드 이름의 인덱스를 가져옵니다.
			*
			* param[in] strFieldName : 필드 명
			* param[out] nIndex : 인덱스 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetFieldIndex(_In_ CString strFieldName, _Out_ int& nIndex);
			
			/**
			* 입력 인덱스의 필드명을 설정합니다.
			*
			* param[in] nIndex : 인덱스
			* param[out] strNewFieldName : 필드 명
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetFieldName(_In_ int nIndex, _In_ CString strNewFieldName);
			
			/**
			* 입력 인덱스의 필드명을 가져옵니다.
			*
			* param[in] nIndex : 인덱스
			* param[out] strFieldName : 필드 명 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetFieldName(_In_ int nIndex, _Out_ CString& strFieldName);
			
			/**
			* 입력 인덱스의 필드 데이터 타입을 설정합니다.
			*
			* param[in] nIndex : 인덱스
			* param[in] strNewFieldType : 데이터 타입
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetFieldType(_In_ int nIndex, _In_ CString strNewFieldType);
			
			/**
			* 입력 인덱스의 필드 데이터 타입을 설정합니다.
			*
			* param[in] nIndex : 인덱스
			* param[in] eType : 데이터 타입
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetFieldType(_In_ int nIndex, _In_ ERavidRecordsetFieldType eType);
			
			/**
			* 입력 인덱스의 필드 데이터 타입을 가져옵니다.
			*
			* param[in] nIndex : 인덱스
			* param[out] eType : 데이터 타입 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetFieldType(_In_ int nIndex, _Out_ ERavidRecordsetFieldType& eType);
			
			/**
			* 입력 인덱스의 필드 데이터 타입을 가져옵니다.
			*
			* param[in] nIndex : 인덱스
			* param[out] strType : 데이터 타입 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetFieldType(_In_ int nIndex, _Out_ CString& strType);
			
			/**
			* 입력 필드의 데이터 타입을 가져옵니다.
			*
			* param[in] strFieldName : 필드 명
			* param[out] eType : 데이터 타입 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetFieldType(_In_ CString strFieldName, _Out_ ERavidRecordsetFieldType& eType);
			
			/**
			* 입력 필드의 데이터 타입을 가져옵니다.
			*
			* param[in] strFieldName : 필드 명
			* param[out] strType : 데이터 타입 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetFieldType(_In_ CString strFieldName, _Out_ CString& strType);
			
			/**
			* 입력 인덱스에 필드를 추가합니다.
			*
			* param[in] nIndex : 인덱스
			* param[in] strFieldName : 필드 명
			* param[in] eType : 데이터 타입
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool InsertField(_In_ int nIndex, _In_ CString strFieldName, _In_ ERavidRecordsetFieldType eType);
			
			/**
			* 입력 인덱스에 필드를 삭제합니다.
			*
			* param[in] nIndex : 인덱스
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool DeleteField(_In_ int nIndex);
			
			/**
			* 필드 데이터 타입을 enum에서 문자열로 변환합니다.
			*
			* param[in] eType : 데이터 타입
			* param[out] strType : 데이터 타입 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool ConvertTypeEnumToTypeString(_In_ ERavidRecordsetFieldType eType, _Out_ CString& strType);
			
			/**
			* 필드 데이터 타입을 문자열에서 enum으로 변환합니다.
			*
			* param[in] strType : 데이터 타입
			* param[out] eType : 데이터 타입 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool ConvertTypeStringToTypeEnum(_In_ CString strType, _Out_ ERavidRecordsetFieldType& eType);
			
			/**
			* 레코드 리스트의 시작인지 확인합니다.
			*
			* return value : bool 의 필드 시작 여부를 반환합니다.
			*/
			bool IsBOF();
			
			/**
			* 레코드 리스트의 끝인지 확인합니다.
			*
			* return value : bool 의 필드 끝 여부를 반환합니다.
			*/
			bool IsEOF();
			
			/**
			* 레코드 리스트의 시작으로 이동합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool MoveFirst();
			
			/**
			* 레코드 리스트의 다음 위치로 이동합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool MoveNext();
			
			/**
			* 레코드 리스트의 이전 위치로 이동합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool MovePrev();
			
			/**
			* 레코드 리스트의 마지막으로 이동합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool MoveLast();
			
			/**
			* 레코드의 수를 가져옵니다.
			*
			* return value : size_t 의 레코드 수를 반환합니다.
			*/
			size_t GetRecordIndex();
			
			/**
			* 현재 레코드에서 필드 인덱스의 데이터 값을 가져옵니다.
			*
			* param[in] nFieldIndex : 필드 인덱스
			* param[out] strValue : 데이터 값 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetFieldValue(_In_ int nFieldIndex, _Out_ CString& strValue);
			
			/**
			* 현재 레코드에서 필드 인덱스의 데이터 값을 가져옵니다.
			* 직접 해당 데이터 타입으로 변경해야 합니다.
			*
			* param[in] nFieldIndex : 필드 인덱스
			* param[out] ppValue : 데이터 값 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetFieldValue(_In_ int nFieldIndex, _Out_ void** ppValue);
						
			/**
			* 현재 레코드에서 필드 명의 데이터 값을 가져옵니다.
			*
			* param[in] strFieldName : 필드 명
			* param[out] strValue : 데이터 값 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetFieldValue(_In_ CString strFieldName, _Out_ CString& strValue);
			
			/**
			* 현재 레코드에서 필드 명의 데이터 값을 가져옵니다.
			* 직접 해당 데이터 타입으로 변경해야 합니다.
			*
			* param[in] strFieldName : 필드 명
			* param[out] ppValue : 데이터 값 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetFieldValue(_In_ CString strFieldName, _Out_ void** ppValue);
			
			/**
			* 현재 레코드에서 필드 인덱스의 데이터 값을 설정합니다.
			*
			* param[in] nIndex : 필드 인덱스
			* param[in] strValue : 데이터 값
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetFieldValue(_In_ int nIndex, _In_ CString strValue);
			
			/**
			* 현재 레코드에서 필드 인덱스의 데이터 값을 설정합니다.
			* 필드 타입에 맞게 설정해야 합니다.
			*
			* param[in] nIndex : 필드 인덱스
			* param[in] pValue : 데이터 값 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetFieldValue(_In_ int nIndex, _In_ void* pValue);
			
			/**
			* 현재 레코드에서 필드 명의 데이터 값을 설정합니다.
			*
			* param[in] strFieldName : 필드 명
			* param[in] strValue : 데이터 값
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetFieldValue(_In_ CString strFieldName, _In_ CString strValue);
			
			/**
			* 현재 레코드에서 필드 명의 데이터 값을 설정합니다.
			* 필드 타입에 맞게 설정해야 합니다.
			*
			* param[in] strFieldName : 필드 명
			* param[in] pValue : 데이터 값
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetFieldValue(_In_ CString strFieldName, _In_ void* pValue);
			
			/**
			* 레코드를 추가합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool AddRecord();
			
			/**
			* 레코드를 삭제합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool DeleteRecord();

		private:

			CRavidRecordset(_In_ CString strTableName, _In_ CRavidDatabase* pParentDataStorage);

			std::unique_ptr<CInternalRavidRecordset> m_pInternal;

			friend class CRavidDatabase;

			friend class CDBEditorManager;
		};
	}
}

