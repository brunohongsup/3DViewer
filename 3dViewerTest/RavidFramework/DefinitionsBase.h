#pragma once

#define WAITFORMESSAGEPUMPING() { MSG msg; while(::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)) { AfxGetApp()->PumpMessage(); } }

namespace Ravid
{
	namespace Framework
	{
		enum EFrameworkLogTypes
		{
			EFrameworkLogTypes_User = 0,
			EFrameworkLogTypes_Ravid = 0x10000,
			EFrameworkLogTypes_Device,
			EFrameworkLogTypes_Model,
			EFrameworkLogTypes_Authority,
			EFrameworkLogTypes_Algorithms,
			EFrameworkLogTypes_Mathematics,
			EFrameworkLogTypes_Miscellaneous,
			EFrameworkLogTypes_Database,
		};

		enum ERavidLogImportanceType
		{
			ERavidLogImportanceType_Normal = 0,
			ERavidLogImportanceType_Warning,
			ERavidLogImportanceType_Error,
			ERavidLogImportanceType_Count,
		};

		typedef struct sRavidLog
		{
			CTime timeLog = 0;
			long nType = 0;
			long nImportanceType = 0;
			CString strMessage = _T("");

			sRavidLog() = default;
			sRavidLog(const sRavidLog&) = default;
			sRavidLog(const sRavidLog* const pRL)
			{
				timeLog = pRL->timeLog;
				nType = pRL->nType;
				nImportanceType = pRL->nImportanceType;
				strMessage = pRL->strMessage;
			}
		}
		SRavidLogData;

		typedef struct sRavidLogImportanceInfo
		{
			CString strName = _T("");
			bool bIsCheck = false;

			sRavidLogImportanceInfo() = default;
			sRavidLogImportanceInfo(const sRavidLogImportanceInfo&) = default;
			sRavidLogImportanceInfo(const sRavidLogImportanceInfo* const pRLCI)
			{
				strName = pRLCI->strName;
				bIsCheck = pRLCI->bIsCheck;
			}
		}
		SRavidLogImportanceInfoData;



		enum EUserType
		{
			EUserType_NotLoggedOn = 0,
			EUserType_Normal,
			EUserType_PowerUser,
			EUserType_Administrator,
			EUserType_Developer,
			EUserType_Root,
			EUserType_Count,
		};

		enum EAuthResult
		{
			EAuthResult_Success = 0,
			EAuthResult_UnknownError,
			EAuthResult_UserNotFound,
			EAuthResult_DatabaseError,
			EAuthResult_IncorrectPassword,
			EAuthResult_NotLoggedOn,
			EAuthResult_AccessDenied,
			EAuthResult_UserAlreadyExist,
			EAuthResult_UserAlreadyLogedOn,
		};

		typedef struct sUserInfo
		{
			CString strUserName;
			EUserType eUserType;
		}
		SUserInfo;


		enum EMessage
		{
			EMessage_LanguageChange = 0x0523,
			EMessage_ImageViewMouseUp = 0x0617,
		};
	}
}

namespace Ravid
{
	namespace Framework
	{
		enum EDeviceLibraryStatus
		{
			EDeviceLibraryStatus_NotFound = 0,
			EDeviceLibraryStatus_FoundTrue,
			EDeviceLibraryStatus_FoundFalse,
		};
	}
}
