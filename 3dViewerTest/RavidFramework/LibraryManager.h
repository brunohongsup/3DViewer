#pragma once

#include "RavidObjectEx.h"

#include <vector>
#include <memory>

namespace Ravid
{
	namespace Framework
	{
		/**
		* 외부 라이브러리(dll) 상태를 관리하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CLibraryManager : public CRavidObjectEx
		{
		private:
			CLibraryManager();

		public:
			virtual ~CLibraryManager();
			
			/**
			* 외부 라이브러리 목록 및 상태를 초기화합니다.
			*/
			static bool Initialize();
			
			/**
			* 외부 라이브러리 목록을 해제합니다.
			*/
			static bool Terminate();
			
			/**
			* 외부 라이브러리 관리 클래스의 초기화 상태를 가져옵니다.
			*/
			static bool IsInitialized();

			static EDeviceLibraryStatus GetFoundLibrary(CString strLib);
			static bool SetFoundLibrary(CString strLib, bool bStatus);

		protected:
			static CLibraryManager* GetInstance();

			bool m_bInitialized = false;
			std::vector<std::pair<CString, bool>> m_vctLibraryStatus;

		};
	}
}
