#pragma once

namespace Ravid
{
	namespace Database
	{
		/**
		* 레코드 필드 정보를 저장하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CRavidRecordsetFieldInformation
		{
		private:
			CRavidRecordsetFieldInformation();

		public:
			CRavidRecordsetFieldInformation(CString strFieldName, ERavidRecordsetFieldType eType);
			virtual ~CRavidRecordsetFieldInformation();
			
			/**
			* 필드 명을 설정합니다.
			*
			* param[in] strFieldName : 필드 명
			*/
			void SetFieldName(_In_ CString strFieldName);
			
			/**
			* 설정된 필드 명을 가져옵니다.
			*
			* return value : CString 의 필드 명을 반환합니다.
			*/
			CString GetFieldName();
			
			/**
			* 필드 데이터 타입을 설정합니다.
			*
			* param[in] eType : 필드 데이터 타입
			*/
			void SetFieldType(_In_ ERavidRecordsetFieldType eType);
			
			/**
			* 설정된 데이터 타입을 가져옵니다.
			*
			* return value : ERavidRecordsetFieldType 의 필드 데이터 타입을 반환합니다.
			*/
			ERavidRecordsetFieldType GetFieldType();

		private:
			CString m_strFieldName;
			ERavidRecordsetFieldType m_eType;
		};
	}
}
