#pragma once

#include "RavidObjectEx.h"

#include <memory>
#include <queue>
#include <map>

namespace Ravid
{
	namespace Framework
	{
		/**
		* 프레임워크 로그를 관리하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CLogManager : public CRavidObjectEx
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CLogManager);

		private:
			CLogManager();

		public:
			virtual ~CLogManager();
			
			/**
			* 프레임워크 로그의 설정을 초기화합니다.
			*/
			static bool Initialize(_In_ CString strLoggingRootFolder);
			
			/**
			* 프레임워크 로그의 설정을 해제합니다.
			*/
			static bool Terminate();
			
			/**
			* 프레임워크 로그의 초기화 상태를 가져옵니다.
			*/
			static bool IsInitialized();
			
			/**
			* 로그 저장 폴더를 설정합니다.
			*/
			static void SetRootPath(_In_ CString strLoggingRootFolder);
			
			/**
			* 로그 저장 폴더를 가져옵니다.
			*/
			static CString GetRootPath();
			
			/**
			* 숫자의 로그 타입 이름을 설정합니다.
			*/
			static bool SetLogTypeName(_In_ long nLogType, _In_ CString strLogTypeName);
			
			/**
			* 숫자의 로그 타입 이름을 가져옵니다.
			*/
			static CString GetLogTypeName(_In_ long nLogType);
			
			/**
			* 로그 타입으로 기록합니다.
			*/
			static bool Write(_In_ long nLogType, _In_ CString strFormat, ...);
			
			/**
			* 로그 타입과 중요도로 기록합니다.
			*/
			static bool Write(_In_ long nLogType, _In_ long nImportanceType, _In_ CString strFormat, ...);
			
			/**
			* 로그 기록을 On / Off 합니다.
			*/
			static void EnableWrite(_In_ bool bEnable);

		private:
			static UINT WritingThread(_In_ LPVOID pParam);

			static CLogManager* GetInstance();

		protected:
			CWinThread* m_pWritingThread = nullptr;
			volatile bool m_bInitialized = false;
			CString m_strLoggingRootFolder;
			std::queue<SRavidLogData*> m_queLogData;
			std::map<long, CString> m_mapLogTypes;

			bool m_bEnableWrite = true;
		};
	}
}

