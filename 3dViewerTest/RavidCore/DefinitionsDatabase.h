#pragma once

enum ERavidRecordsetFieldType
{
	ERavidRecordsetFieldType_Undefined = 0,
	ERavidRecordsetFieldType_Bool,
	ERavidRecordsetFieldType_Char,
	ERavidRecordsetFieldType_UnsignedChar,
	ERavidRecordsetFieldType_Short,
	ERavidRecordsetFieldType_UnsignedShort,
	ERavidRecordsetFieldType_Long,
	ERavidRecordsetFieldType_UnsignedLong,
	ERavidRecordsetFieldType_LongLong,
	ERavidRecordsetFieldType_UnsignedLongLong,
	ERavidRecordsetFieldType_Float,
	ERavidRecordsetFieldType_Double,
	ERavidRecordsetFieldType_String,
	ERavidRecordsetFieldTypeCount,
};

enum ERavidDBWhereClauseOperator
{
	ERavidDBWhereClauseOperator_Unknown = 0,
	ERavidDBWhereClauseOperator_And,
	ERavidDBWhereClauseOperator_Or,
	ERavidDBWhereClauseOperator_Greater,
	ERavidDBWhereClauseOperator_GreaterEqual,
	ERavidDBWhereClauseOperator_Less,
	ERavidDBWhereClauseOperator_LessEqual,
	ERavidDBWhereClauseOperator_Equal,
	ERavidDBWhereClauseOperator_NotEqual,
	ERavidDBWhereClauseOperator_OpenBracket,
	ERavidDBWhereClauseOperator_CloseBracket,
};

enum ERavidRecordsetOrderBy
{
	ERavidRecordsetOrderBy_Asc = 0,
	ERavidRecordsetOrderBy_Desc,
};

typedef struct sRavidDBWhereClauseCondition
{
	CString strField;
	int nFieldIndex;
	ERavidDBWhereClauseOperator eOperator;
	CString strValue;
}
SRavidDBWhereClauseCondition;

enum ERavidDatabaseResult
{
	ERavidDatabaseResult_OK = 0,
	ERavidDatabaseResult_UnknownError,
	ERavidDatabaseResult_FileNotFound,
	ERavidDatabaseResult_CouldNotOpen,
	ERavidDatabaseResult_InvalidPassword,
};

enum ERavidRecordsetResult
{
	ERavidRecordsetResult_OK = 0,
	ERavidRecordsetResult_UnknownError,
};
