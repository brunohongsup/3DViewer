#pragma once

#include "RavidObjectEx.h"

#include <memory>
#include <vector>

namespace Ravid
{
	namespace Framework
	{
		/**
		* 프레임워크 권한 관리 클래스입니다.
		*/
		class AFX_EXT_CLASS CAuthorityManager : public CRavidObjectEx
		{
		public:
			enum EPermissionDataType
			{
				EPermissionDataType_DeviceItemEdit = 0,
				EPermissionDataType_DeviceControl,
				EPermissionDataType_ModelItemEdit,
				EPermissionDataType_ModelOpen,
				EPermissionDataType_ModelClose,
				EPermissionDataType_ModelNew,
				EPermissionDataType_ModelCopy,
				EPermissionDataType_ModelDelete,
				EPermissionDataType_ModelConfigurationAccess,
				EPermissionDataType_ModelConfigurationNew,
				EPermissionDataType_ModelConfigurationModify,
				EPermissionDataType_ModelConfigurationDelete,
				EPermissionDataType_ModelConfigurationDuplicate,
				EPermissionDataType_ImageProcessingTool,
				EPermissionDataType_ConfigurationSave,
				EPermissionDataType_ConfigurationSaveas,
				EPermissionDataType_ConfigurationNew,
				EPermissionDataType_ConfigurationDelete,
				EPermissionDataType_ConfigurationRename,
				EPermissionDataType_ChildFrameSize,
				EPermissionDataType_ImageViewAccuracyEdit,
				EPermissionDataType_ImageViewLayer,
				EPermissionDataType_Count
			};

			typedef struct sLayerPermission
			{
				long nViewObjectID = 0;
				long nLayerIndex = 0;
				EUserType ePermission = EUserType_NotLoggedOn;
			}
			SLayerPermission;

		private:
			CAuthorityManager();

		public:
			virtual ~CAuthorityManager();
			
			/**
			* 프레임워크 전체에 대한 권한 관리를 초기화합니다.
			*/
			static bool Initialize();

			/**
			* 권한 관리를 해제합니다.
			*/
			static bool Terminate();
			
			/**
			* 권한 관리의 초기화 상태를 확인합니다.
			*/
			static bool IsInitialized();
			
			/**
			* 권한 관리 클래스 인스턴스를 가져옵니다.
			*/
			static CAuthorityManager* GetInstance();
			
			/**
			* 현재 로그인된 사용자의 이름을 가져옵니다.
			*/
			static CString GetCurrentUserName();
			
			/**
			* 현재 로그인된 사용자의 권한 타입을 가져옵니다.
			*/
			static EUserType GetCurrentUserType();
			
			/**
			* 로그인 합니다.
			*/
			static EAuthResult LogIn(_In_ CString strUserName, _In_ CString strPassword);
			
			/**
			* 로그아웃 합니다.
			*/
			static EAuthResult LogOut();
			
			/**
			* 사용자 비밀번호를 변경합니다.
			*/
			static EAuthResult ModifyPassword(_In_ CString strUserName, _In_ CString strOldPassword, _In_ CString strNewPassword);

			// It can't be changed to a level higher than currently logged on user
			/**
			* 사용자 등급을 변경합니다.
			*/
			static EAuthResult ModifyUserType(_In_ CString strUserName, EUserType eUserType);

			// Functions that can be used above administrator level			
			/**
			* 사용자를 추가합니다.
			*/
			static EAuthResult AddUser(_In_ CString strUserName, _In_ CString strPassword, _In_ EUserType eUserType);
			
			/**
			* 사용자를 삭제합니다.
			*/
			static EAuthResult DeleteUser(_In_ CString strUserName, _In_ CString strPassword);
			
			/**
			* 등록된 사용자 수를 가져옵니다.
			*/
			static long GetUserCount();
			
			/**
			* 디바이스 파라메터 수정 권한을 설정합니다.
			*/
			static EAuthResult SetDeviceItemEditPermission(_In_ EUserType eUserType);
			
			/**
			* 디바이스 파라메터 수정 권한을 가져옵니다.
			*/
			static EUserType GetDeviceItemEditPermission();
			
			/**
			* 디바이스 제어 권한을 설정합니다.
			*/
			static EAuthResult SetDeviceControlPermission(_In_ EUserType eUserType);
			
			/**
			* 디바이스 제어 권한을 가져옵니다.
			*/
			static EUserType GetDeviceControlPermission();
			
			/**
			* 모델 파라메터 수정 권한을 설정합니다.
			*/
			static EAuthResult SetModelItemEditPermission(_In_ EUserType eUserType);
			
			/**
			* 모델 파라메터 수정 권한을 가져옵니다.
			*/
			static EUserType GetModelItemEditPermission();
			
			/**
			* 모델 파라메터 Open 권한을 설정합니다.
			*/
			static EAuthResult SetModelOpenPermission(_In_ EUserType eUserType);
			
			/**
			* 모델 파라메터 Open 권한을 가져옵니다.
			*/
			static EUserType GetModelOpenPermission();
			
			/**
			* 모델 파라메터 Close 권한을 설정합니다.
			*/
			static EAuthResult SetModelClosePermission(_In_ EUserType eUserType);
			
			/**
			* 모델 파라메터 Close 권한을 가져옵니다.
			*/
			static EUserType GetModelClosePermission();
			
			/**
			* 모델 파라메터 생성 권한을 설정합니다.
			*/
			static EAuthResult SetModelNewPermission(_In_ EUserType eUserType);
			
			/**
			* 모델 파라메터 생성 권한을 가져옵니다.
			*/
			static EUserType GetModelNewPermission();
			
			/**
			* 모델 파라메터 복사 권한을 설정합니다.
			*/
			static EAuthResult SetModelCopyPermission(_In_ EUserType eUserType);
			
			/**
			* 모델 파라메터 복사 권한을 가져옵니다.
			*/
			static EUserType GetModelCopyPermission();
			
			/**
			* 모델 파라메터 삭제 권한을 설정합니다.
			*/
			static EAuthResult SetModelDeletePermission(_In_ EUserType eUserType);
			
			/**
			* 모델 파라메터 삭제 권한을 가져옵니다.
			*/
			static EUserType GetModelDeletePermission();
			
			/**
			* 이미지 툴 사용 권한을 설정합니다.
			*/
			static EAuthResult SetImageProcessingToolPermission(_In_ EUserType eUserType);
			
			/**
			* 이미지 툴 사용 권한을 가져옵니다.
			*/
			static EUserType GetImageProcessingToolPermission();
			
			/**
			* 자식 프레임 크기 변경 권한을 설정합니다.
			*/
			static EAuthResult SetChildFrameSizePermission(_In_ EUserType eUserType);
			
			/**
			* 자식 프레임 크기 변경 권한을 가져옵니다.
			*/
			static EUserType GetChildFrameSizePermission();
			
			/**
			* 이미지 뷰 분해능 변경 권한을 설정합니다.
			*/
			static EAuthResult SetImageViewAccuracyEditPermission(_In_ EUserType eUserType);
			
			/**
			* 이미지 뷰 분해능 변경 권한을 가져옵니다.
			*/
			static EUserType GetImageViewAccuracyEditPermission();
			
			/**
			* 이미지 뷰 레이어 출력 권한을 설정합니다.
			*/
			static EAuthResult SetImageViewLayerPermission(_In_ long nViewObjectID, _In_ long nLayerIndex, _In_ EUserType eUserType);
			
			/**
			* 이미지 뷰 레이어 출력 권한을 가져옵니다.
			*/
			static EUserType GetImageViewLayerPermission(_In_ long nViewObjectID, _In_ long nLayerIndex);
			
			/**
			* 사용자가 존재하는지 확인합니다.
			*/
			static EAuthResult CheckUser(_In_ CString strUserName, _In_ CString strPassword);
			
			/**
			* 사용자 리스트를 가져옵니다.
			*/
			static EAuthResult GetUserInfoList(std::vector<SUserInfo>& vctInfoList);
			
			/**
			* 사용자 리스트를 가져옵니다.
			*/
			static EAuthResult GetUserInfoList(std::vector<SUserInfo>* pVctInfoList);
			
			/**
			* root 사용자가 등록되어 있는지 확인합니다.
			*/
			static EAuthResult IsBeingRoot();
			
			/**
			* 화면 구성 저장 권한을 설정합니다.
			*/
			static EAuthResult SetConfigurationSavePermission(_In_ EUserType eUserType);
			
			/**
			* 화면 구성 저장 권한을 가져옵니다.
			*/
			static EUserType GetConfigurationSavePermission();
			
			/**
			* 화면 구성 다른이름 저장 권한을 설정합니다.
			*/
			static EAuthResult SetConfigurationSaveasPermission(_In_ EUserType eUserType);
			
			/**
			* 화면 구성 다른이름 저장 권한을 가져옵니다.
			*/
			static EUserType GetConfigurationSaveasPermission();
			
			/**
			* 화면 구성 생성 권한을 설정합니다.
			*/
			static EAuthResult SetConfigurationNewPermission(_In_ EUserType eUserType);
			
			/**
			* 화면 구성 생성 권한을 가져옵니다.
			*/
			static EUserType GetConfigurationNewPermission();
			
			/**
			* 화면 구성 삭제 권한을 설정합니다.
			*/
			static EAuthResult SetConfigurationDeletePermission(_In_ EUserType eUserType);
			
			/**
			* 화면 구성 삭제 권한을 가져옵니다.
			*/
			static EUserType GetConfigurationDeletePermission();
			
			/**
			* 화면 구성 이름변경 권한을 설정합니다.
			*/
			static EAuthResult SetConfigurationRenamePermission(_In_ EUserType eUserType);
			
			/**
			* 화면 구성 이름변경 권한을 가져옵니다.
			*/
			static EUserType GetConfigurationRenamePermission();
			
			/**
			* 화면 구성 접근 권한을 설정합니다.
			*/
			static EAuthResult SetModelConfigurationAccessPermission(_In_ EUserType eUserType);
			
			/**
			* 화면 구성 접근 권한을 가져옵니다.
			*/
			static EUserType GetModelConfigurationAccessPermission();
			
			/**
			* 모델 구조 생성 권한을 설정합니다.
			*/
			static EAuthResult SetModelConfigurationNewPermission(_In_ EUserType eUserType);
			
			/**
			* 모델 구조 생성 권한을 가져옵니다.
			*/
			static EUserType GetModelConfigurationNewPermission();
			
			/**
			* 모델 구조 수정 권한을 설정합니다.
			*/
			static EAuthResult SetModelConfigurationModifyPermission(_In_ EUserType eUserType);
			
			/**
			* 모델 구조 수정 권한을 가져옵니다.
			*/
			static EUserType GetModelConfigurationModifyPermission();
			
			/**
			* 모델 구조 삭제 권한을 설정합니다.
			*/
			static EAuthResult SetModelConfigurationDeletePermission(_In_ EUserType eUserType);
			
			/**
			* 모델 구조 삭제 권한을 가져옵니다.
			*/
			static EUserType GetModelConfigurationDeletePermission();
			
			/**
			* 모델 구조 복사 권한을 설정합니다.
			*/
			static EAuthResult SetModelConfigurationDuplicatePermission(_In_ EUserType eUserType);
			
			/**
			* 모델 구조 복사 권한을 가져옵니다.
			*/
			static EUserType GetModelConfigurationDuplicatePermission();

		private:
			static EAuthResult SetPermissionData(_In_ EPermissionDataType eType, _In_ CString strData, _In_opt_ CString strSubTitle = _T(""));
			static EAuthResult GetPermissionData(_In_ EPermissionDataType eType, _Out_ CString& strData, _In_opt_ CString strSubTitle = _T(""));

		protected:
			bool m_bInitialized = false;
			CString m_strCurrentUser = _T("");
			EUserType m_eUserType = EUserType_NotLoggedOn;
			static LPCTSTR m_lpszPermissionDataType[CAuthorityManager::EPermissionDataType_Count];
			EUserType m_ePermission[CAuthorityManager::EPermissionDataType_Count];
			std::vector<CAuthorityManager::SLayerPermission> m_vctLayerPermission;
		};
	}
}

