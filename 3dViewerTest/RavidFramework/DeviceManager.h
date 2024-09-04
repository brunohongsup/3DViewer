#pragma once

#include "RavidObjectEx.h"

#include <vector>
#include <memory>

namespace Ravid
{
	namespace Device
	{
		class CDeviceBase;
		
		/**
		* 프레임워크 디바이스 관리 클래스입니다.
		*/
		class AFX_EXT_CLASS CDeviceManager : public Framework::CRavidObjectEx
		{
		private:
			CDeviceManager();

		public:
			virtual ~CDeviceManager();
			
			/**
			* 프레임워크 전체 사용 가능한 디바이스에 대해 초기화합니다.
			* 등록된 디바이스를 리스트업 합니다.
			*/
			static bool Initialize();
			
			/**
			* 프레임워크 전체 사용 가능한 디바이스에 대해 해제합니다.
			* 등록된 디바이스를 해제 후 삭제합니다.
			*/
			static bool Terminate();
			
			/**
			* 디바이스 관리 클래스의 초기화 상태를 확인합니다.
			*/
			static bool IsInitialized();
			
			/**
			* 등록된 디바이스의 수를 가져옵니다.
			*/
			static size_t GetDeviceCount();
			
			/**
			* 해당 인덱스로 디바이스를 가져옵니다.
			*/
			static CDeviceBase* GetDeviceByIndex(_In_ size_t stIndex);
			
			/**
			* 해당 클래스 & 인덱스로 디바이스를 가져옵니다.
			*/
			static CDeviceBase* GetDeviceByIndex(_In_ CRuntimeClass* pRuntimeClass, _In_opt_ size_t stIndex = 0);
			
			/**
			* 해당 클래스 타입 & 인덱스로 디바이스를 가져옵니다.
			*/
			static CDeviceBase* GetDeviceByIndex(_In_ const type_info& typeInfo, _In_opt_ size_t stIndex = 0);
						
			/**
			* 해당 클래스 이름 & 인덱스로 디바이스를 가져옵니다.
			*/
			static CDeviceBase* GetDeviceByIndex(_In_ CString strClassNameIncludingNameSpace, _In_opt_ size_t stIndex = 0);
			
			/**
			* 해당 ID로 디바이스를 가져옵니다.
			*/
			static CDeviceBase* GetDeviceByObjectID(_In_ long nObjectID);
			
			/**
			* 해당 클래스 & ID로 디바이스를 가져옵니다.
			*/
			static CDeviceBase* GetDeviceByObjectID(_In_ CRuntimeClass* pRuntimeClass, _In_opt_ long nObjectID = -1);
			
			/**
			* 해당 클래스 타입 & ID로 디바이스를 가져옵니다.
			*/
			static CDeviceBase* GetDeviceByObjectID(_In_ const type_info& typeInfo, _In_opt_ long nObjectID = -1);
			
			/**
			* 해당 클래스 이름 & ID로 디바이스를 가져옵니다.
			*/
			static CDeviceBase* GetDeviceByObjectID(_In_ CString strClassNameIncludingNameSpace, _In_opt_ long nObjectID = -1);
			
			/**
			* 해당 클래스 인덱스로 디바이스를 속성창을 활성화합니다.
			*/
			static bool ShowDevicePropertyPage(_In_opt_ size_t stIndex = 0, _In_opt_ CDeviceBase* pCurrentDevice = nullptr);
			
			/**
			* 프레임워크에서 제공하는 디바이스 리스트를 가져옵니다.
			*/
			static std::vector<CDeviceBase*>* GetAvailableDevices();
			
			/**
			* 등록된 디바이스 리스트를 가져옵니다.
			*/
			static bool GetRegisterDeviceInfos(_Out_ std::vector<SRegisterDeviceInfo>& vctInfos);
			
			/**
			* 등록된 디바이스 리스트를 가져옵니다.
			*/
			static bool GetRegisterDeviceInfos(_Out_ std::vector<SRegisterDeviceInfo>* pVctInfos);
			
			/**
			* 디바이스 리스트를 등록합니다.
			*/
			static bool UpdateRegisterDeviceInfosTable(_In_ std::vector<SRegisterDeviceInfo>& vctInfos);
			
			/**
			* 디바이스 리스트를 등록합니다.
			*/
			static bool UpdateRegisterDeviceInfosTable(_In_ std::vector<SRegisterDeviceInfo>* pVctInfos);
			
			/**
			* 사용가능한 디바이스 목록에 등록합니다.
			*/
			static bool RegisterDeviceInfos(_In_ CDeviceBase* pDevice);
			
			/**
			* 사용가능한 디바이스 목록에 해당 클래스 이름이 있는지 확인합니다.
			*/
			static CDeviceBase* IsRegisterDeviceInfos(_In_ CString strDeviceName);

		private:
			static CDeviceManager* GetInstance();

			static bool IsAvailableRegisterDeviceInfos();
			static bool CreateRegisterDeviceInfosTable(_In_opt_ bool bSave = true);
			static bool DropRegisterDeviceInfosTable(_In_opt_ bool bSave = true);

			static bool RegisterDevice(_In_ CDeviceBase* pDevice, _In_opt_ bool bAutoDestroy = true);

			static bool UnregisterDevice(_In_ size_t stIndex);
			static bool UnregisterDevice(_In_ CDeviceBase* pDevice);

			static CDeviceBase* FindAvailableDevice(_In_ CString strClassName);

		protected:
			bool m_bInitialized = false;
			std::vector<std::pair<CDeviceBase*, bool> > m_vctDevices;
			std::vector<CDeviceBase*> m_vctVisibleDevices;
			std::vector<CDeviceBase*> m_vctAvailableDevices;

		};
	}
}
