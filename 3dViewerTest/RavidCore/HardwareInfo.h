#pragma once

namespace Ravid
{
	namespace Miscellaneous
	{
		/**
		* 현재 하드웨어 정보를 가져오는 클래스입니다.
		*/
		class AFX_EXT_CLASS CHardwareInfo : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CHardwareInfo);

		public:
			CHardwareInfo();
			virtual ~CHardwareInfo();
			
			/**
			* CPU 에서 지원하는 명령어 세트를 가져옵니다.
			*
			* return value : ECPUInstructionType enum 의 결과를 반환합니다.
			*	SSE, SSE2, SSE3, SSE4, XOP, AVX, AVX2 등 조합된 결과를 반환합니다.
			*/
			static ECPUInstructionType GetCPUInstruction();
			
			/**
			* 설치된 총 메모리 크기를 가져옵니다.
			*
			* return value : ULONGLONG 의 총 메모리 크기를 반환합니다.
			*/
			static ULONGLONG GetTotalMemory();
			
			/**
			* 현재 사용 가능한 메모리 크기를 가져옵니다.
			*
			* return value : ULONGLONG 의 총 메모리 크기를 반환합니다.
			*/
			static ULONGLONG GetAvailableMemory();
			
			/**
			* 현재 CPU 사용량을 가져옵니다.
			*
			* return value : double 의 CPU 사용량을 반환합니다.
			*/
			static double GetCPUUsage();
			
			/**
			* 물리적 패키지의 수를 가져옵니다.
			* 논리 프로세서는 물리적 패키지를 공유합니다.
			*
			* return value : int 의 코어 수를 반환합니다.
			*/
			static int GetProcessorPackageCount();
			
			/**
			* CPU 소켓의 수를 가져옵니다.
			*
			* return value : int 의 코어 수를 반환합니다.
			*/
			static int GetNumaNodeCount();
			
			/**
			* 물리적 프로세서의 수를 가져옵니다.
			*
			* return value : int 의 캐시 수를 반환합니다.
			*/
			static int GetPhysicalProcessorCoreCount();
			
			/**
			* 논리적 프로세서의 수를 가져옵니다.
			*
			* return value : int 의 캐시 수를 반환합니다.
			*/
			static int GetLogicalProcessorCoreCount();
			
			/**
			* L1 캐시 수를 가져옵니다.
			* 보통 물리적 코어 수의 2배 (데이터 캐시, 명령 캐시) 입니다.
			*
			* return value : int 의 캐시 수를 반환합니다.
			*/
			static int GetProcessorL1CacheCount();
			
			/**
			* L2 캐시 수를 가져옵니다.
			* 보통 물리적 코어 수와 동일합니다.
			*
			* return value : int 의 캐시 수를 반환합니다.
			*/
			static int GetProcessorL2CacheCount();
			
			/**
			* L3 캐시 수를 가져옵니다.
			* 코어 수와 상관없이 CPU 당 1개 혹은 메인보드 1개로 설정됩니다.
			*
			* return value : int 의 캐시 수를 반환합니다.
			*/
			static int GetProcessorL3CacheCount();
			
			/**
			* L1 캐시 사이즈를 가져옵니다.
			* CPU 가 메모리로부터 데이터를 가져올 때 캐시 사이즈 만큼 사상이 가능합니다.
			*
			* return value : int 의 캐시 사이즈를 반환합니다.
			*/
			static int GetProcessorL1CacheSize();
			
			/**
			* L2 캐시 사이즈를 가져옵니다.
			* CPU 가 메모리로부터 데이터를 가져올 때 캐시 사이즈 만큼 사상이 가능합니다.
			*
			* return value : int 의 캐시 사이즈를 반환합니다.
			*/
			static int GetProcessorL2CacheSize();
			
			/**
			* L3 캐시 사이즈를 가져옵니다.
			* CPU 가 메모리로부터 데이터를 가져올 때 캐시 사이즈 만큼 사상이 가능합니다.
			*
			* return value : int 의 캐시 사이즈를 반환합니다.
			*/
			static int GetProcessorL3CacheSize();
			
			/**
			* L1 캐시라인 사이즈를 가져옵니다.
			* CPU 가 메모리로부터 데이터를 가져올 때 캐시라인 사이즈 만큼 가져옵니다.
			*
			* return value : int 의 캐시라인 사이즈를 반환합니다.
			*/
			static int GetProcessorL1LineSize();
			
			/**
			* L2 캐시라인 사이즈를 가져옵니다.
			* CPU 가 메모리로부터 데이터를 가져올 때 캐시라인 사이즈 만큼 가져옵니다.
			*
			* return value : int 의 캐시라인 사이즈를 반환합니다.
			*/
			static int GetProcessorL2LineSize();
			
			/**
			* L3 캐시라인 사이즈를 가져옵니다.
			* CPU 가 메모리로부터 데이터를 가져올 때 캐시라인 사이즈 만큼 가져옵니다.
			*
			* return value : int 의 캐시라인 사이즈를 반환합니다.
			*/
			static int GetProcessorL3LineSize();
		};
	}
}

