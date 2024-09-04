#pragma once

#include "RavidObjectEx.h"

#include <memory>

namespace Ravid
{
	namespace Database
	{
		class CRavidDatabase;
	}

	namespace Framework
	{
		/**
		* 프레임워크 전체를 관리하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CFrameworkManager : public CRavidObjectEx
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CFrameworkManager);

			enum EFrameworkSettingDataType
			{
				EFrameworkSettingDataType_Security = 0,
				EFrameworkSettingDataType_Count,
			};

			enum EFrameworkSettingLoginData
			{
				EFrameworkSettingLoginData_TryMaxCount = 5,
				EFrameworkSettingLoginData_LockTimes = 300,
			};

		private:
			CFrameworkManager();

		public:
		virtual ~CFrameworkManager();
				
			/**
			* 프레임워크 설정 및 전체 프레임 시작합니다.
			* 모든 메니저를 초기화합니다.
			*/
			static bool Begin(_In_opt_ LPCTSTR lpszSettingFile = nullptr, _In_opt_ bool bSettingMode = false, _In_opt_ UINT uiToolBarResourceEnableID = 0, _In_opt_ UINT uiToolBarResourceDisableID = 0);
			
			/**
			* 프레임워크를 종료합니다.
			*/
			static bool End();
			
			/**
			* 현재 프레임워크 실행 모드가 세팅모드인지 확인합니다.
			*/
			static bool IsSettingMode();
			
			/**
			* 프레임워크 구조를 가지는 데이터베이스를 가져옵니다.
			*/
			static Database::CRavidDatabase* GetFrameworkDatabase();
			
			/**
			* 모델 구조를 가지는 데이터베이스를 가져옵니다.
			*/
			static Database::CRavidDatabase* GetModelDatabase();
			
			/**
			* 디바이스 구조를 가지는 데이터베이스를 가져옵니다.
			*/
			static Database::CRavidDatabase* GetDeviceDatabase();
			
			/**
			* 보안키를 설정합니다.
			*/
			static bool SetSecurityKey(_In_ CString strSecurityKey);
			
			/**
			* 보안키를 변경합니다.
			*/
			static bool ChangeSecurityKey(_In_ CString strCurrentSecurityKey, _In_ CString strNewSecurityKey);
			
			/**
			* 보안키를 설정 여부를 확인합니다.
			*/
			static bool IsSetSecurityKey();
						
			/**
			* 보안 상태로 설정합니다.
			*/
			static bool LockSecurity();
						
			/**
			* 보안 상태를 해제합니다.
			*/
			static bool UnlockSecurity(_In_ CString strSecurityKey);
			
			/**
			* 보안 상태를 확인합니다.
			*/
			static bool IsUnlockedSecurity();
			
			/**
			* 보안 위반 시간을 확인합니다.
			*/
			static bool IsRestrictSecurity(_Out_ CString& strTime);
			
			/**
			* 보안 위반 시간, 횟수를 확인합니다.
			*/
			static bool GetRestrictSecurityTrialInfo(_Out_ int& nCount, _Out_ CString& strTime);
			
			/**
			* 보안 위반 시간, 횟수를 설정합니다.
			*/
			static bool SetRestrictSecurityTrialInfo(_In_ int nCount, _In_ CTime tTime);
			
			/**
			* 시간을 입력하면 CTime 클래스로 변환합니다.
			*/
			static bool ConvertStringToTime(_In_ CString strTime, _Out_ CTime& tTime);
			
			/**
			* 보안 위반 시간을 CTime 클래스로 변환합니다.
			*/
			static CTime GetRestrictSecurityTime(_In_ CTime& tInTimem);
			
			/**
			* 모델 구조 파일의 이름을 설정합니다.
			*/
			static void SetModelSettingFileTitle(LPCTSTR lpszSettingFile);
			
			/**
			* 디바이스 구조 파일의 이름을 설정합니다.
			*/
			static void SetDeviceSettingFileTitle(LPCTSTR lpszSettingFile);
			
			/**
			* 프레임워크 실행 여부를 가져옵니다.
			*/
			static bool GetFrameworkRun();

		private:
			static CFrameworkManager* GetInstance();

			static bool SetFrameworkSettingData(_In_ EFrameworkSettingDataType eType, _In_ CString strData);
			static bool GetFrameworkSettingData(_In_ EFrameworkSettingDataType eType, _Out_ CString& strData);

			static bool ArrangeDeviceSettingData();
			static bool ArrangeModelSettingData();

			static LPCTSTR m_lpszFrameworkSettingDataType[EFrameworkSettingDataType_Count];

		protected:
			Database::CRavidDatabase* m_pDatabase = nullptr;
			Database::CRavidDatabase* m_pModelDatabase = nullptr;
			Database::CRavidDatabase* m_pDeviceDatabase = nullptr;

			bool m_bIsSettingMode = false;
			bool m_bIsSecurity = true;
			bool m_bIsFrameworkRun = false;


			CString m_strModelSettingFile = _T("ModelSettings");
			CString m_strDeviceSettingFile = _T("DeviceSettings");
		};
	}
}

