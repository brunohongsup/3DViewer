#pragma once

namespace Ravid
{
	namespace Miscellaneous
	{
		/**
		* 현재 OS 의 정보를 가져오는 클래스입니다.
		*/
		class AFX_EXT_CLASS COperatingSystemInfo : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(COperatingSystemInfo);

		public:
			COperatingSystemInfo();
			virtual ~COperatingSystemInfo();
			
			/**
			* 현재 동작중인 OS 의 아키텍처를 확인합니다.			
			*
			* return value : bool 의 x64 여부를 반환합니다.
			* x86 은 false, x64 는 true 를 반환합니다.
			*/
			bool IsX64OperatingSystem();
			
			/**
			* 입력된 프로세스의 아키텍처를 확인합니다.
			* 입력된 값이 0일 경우 현재 프로세스의 결과를 확인합니다.
			*
			* param[in] hProcess : 프로세스 핸들값 (Default = 0)
			* 
			* return value : bool 의 x64 여부를 반환합니다.
			* x86 은 false, x64 는 true 를 반환합니다.
			*/
			bool IsX64Process(HANDLE hProcess = 0);

			EOperatingSystemType GetOperatingSystemType();
		};
	}
}

