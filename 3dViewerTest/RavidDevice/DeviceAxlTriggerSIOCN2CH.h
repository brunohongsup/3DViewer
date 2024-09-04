#pragma once

#ifdef USING_DEVICE

#include "../RavidFramework/DeviceTrigger.h"
#include "../RavidFramework/DefinitionsDevice.h"


namespace Ravid
{
	namespace Device
	{
		class CDeviceAxlTriggerSIOCN2CH : public CDeviceTrigger
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceAxlTriggerSIOCN2CH);

			DECLARE_DYNAMIC(CDeviceAxlTriggerSIOCN2CH)


			CDeviceAxlTriggerSIOCN2CH();
			virtual ~CDeviceAxlTriggerSIOCN2CH();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual bool EnableTrigger() override;
			virtual bool DisableTrigger() override;

			virtual bool IsTriggerEnabled() override;

			virtual long GetEncoderCount() override;
			virtual long GetTriggerCount() override;
			virtual bool ResetCounter() override;

			/**
			* 카운터 모듈의 위치 단위를 설정한다.
			* 실제 위치 이동량에 대한 펄스 갯수를 설정하는데,
			* ex) 1mm 이동에 1000가 필요하다면dMoveUnitPerPulse = 0.001로 입력하고,
			*     이후 모든 함수의 위치와 관련된 값을 mm 단위로 설정하면 된다.
			*/
			virtual EAxlTriggerSIOCN2CHGetFunction GetUnitPerPulse(_Out_ double* pParam);
			virtual EAxlTriggerSIOCN2CHSetFunction SetUnitPerPulse(_In_ double dblParam);

			virtual EAxlTriggerSIOCN2CHGetFunction GetTriggerFunction(_Out_ EDeviceAxlTriggerSIOCN2CTriggerFunction* pParam);
			virtual EAxlTriggerSIOCN2CHSetFunction SetTriggerFunction(_In_ EDeviceAxlTriggerSIOCN2CTriggerFunction eParam);
			
			/**
			* SIO-CN2CH/HPC4의 경우
			* dwMethod --> 0x00 : Up/Down 방식, A phase : 펄스, B phase : 방향
			* dwMethod --> 0x01 : Phase-A and phase-B pulses, x1 multiplication
			* dwMethod --> 0x02 : Phase-A and phase-B pulses, x2 multiplication
			* dwMethod --> 0x03 : Phase-A and phase-B pulses, x4 multiplication
			*/
			virtual EAxlTriggerSIOCN2CHGetFunction GetEncoderInputMethod(_Out_ EDeviceAxlTriggerSIOCN2CHEncoderInputMethod* pParam);
			virtual EAxlTriggerSIOCN2CHSetFunction SetEncoderInputMethod(_In_ EDeviceAxlTriggerSIOCN2CHEncoderInputMethod eParam);

			/**
			* 카운터 모듈의 Encoder 입력 신호를 선택한다.
			* dwSource -->  0x00 : 2(A/B)-Phase 신호.
			* dwSource -->  0x01 : Z-Phase 신호.(방향성 없음.)
			*/ 
			virtual EAxlTriggerSIOCN2CHGetFunction GetEncoderSource(_Out_ EDeviceAxlTriggerSIOCN2CHEncoderSource* pParam);
			virtual EAxlTriggerSIOCN2CHSetFunction SetEncoderSource(_In_ EDeviceAxlTriggerSIOCN2CHEncoderSource eParam);
			
			/**
			* 카운터 모듈의 엔코더 입력 카운터를 반전 기능을 설정한다.
			* dwReverse --> 0x00 : 반전하지 않음.
			* dwReverse --> 0x01 : 반전.
			*/ 
			virtual EAxlTriggerSIOCN2CHGetFunction GetEncoderReverse(_Out_ bool* pParam);
			virtual EAxlTriggerSIOCN2CHSetFunction SetEncoderReverse(_In_ bool bParam);
			
			/**
			* SIO-CN2CH의 경우
			* dwMode -->  0x00 : 절대 위치 트리거 또는 주기 위치 트리거. 
			*/
			virtual EAxlTriggerSIOCN2CHGetFunction GetTriggerMode(_Out_ EDeviceAxlTriggerSIOCN2CHTriggerMode* pParam);
			virtual EAxlTriggerSIOCN2CHSetFunction SetTriggerMode(_In_ EDeviceAxlTriggerSIOCN2CHTriggerMode eParam);
			
			/**
			* 카운터 모듈의 트리거 출력 펄스의 출력 레벨을 설정한다.
			* dwLevel -->  0x00 : 트리거 출력시 'Low' 레벨 출력.
			* dwLevel -->  0x01 : 트리거 출력시 'High' 레벨 출력.
			*/
			virtual EAxlTriggerSIOCN2CHGetFunction GetTriggerActiveLevel(_Out_ EDeviceAxlTriggerSIOCN2CHActiveLevel* pParam);
			virtual EAxlTriggerSIOCN2CHSetFunction SetTriggerActiveLevel(_In_ EDeviceAxlTriggerSIOCN2CHActiveLevel eParam);
			
			/**
			* 카운터 모듈의 위치 주기 모드 트리거 사용시 위치 증감에 대한 유효기능을 설정한다.
			* dwDirection -->  0x00 : 카운터의 증/감에 대하여 트리거 주기 마다 출력.
			* dwDirection -->  0x01 : 카운터가 증가 할때만 트리거 주기 마다 출력.
			* dwDirection -->  0x02 : 카운터가 감소 할때만 트리거 주기 마다 출력.
			*/
			virtual EAxlTriggerSIOCN2CHGetFunction GetTriggerDirectionCheck(_Out_ EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck* pParam);
			virtual EAxlTriggerSIOCN2CHSetFunction SetTriggerDirectionCheck(_In_ EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck eParam);
			
			/**
			* 카운터 모듈의 트리거 출력 펄스 폭을 설정한다.
			* 단위 : uSec
			*/
			virtual EAxlTriggerSIOCN2CHGetFunction GetTriggerPulseWidth(_Out_ double* pParam);
			virtual EAxlTriggerSIOCN2CHSetFunction SetTriggerPulseWidth(_In_ double dblParam);
			
			/**
			* 카운터 모듈의 트리거 출력 범위 중 하한 값을 설정한다.
			* 위치 주기 트리거 제품의 경우 위치 주기로 트리거 출력을 발생시킬 범위 중 하한 값을 설정한다.
			* 절대 위치 트리거 제품의 경우 Ram 시작 번지의 트리거 정보의 적용 기준 위치를 설정한다.
			* 단위 : AxcMotSetMoveUnitPerPulse로 설정한 단위.
			* Note : 하한값은 상한값보다 작은값을 설정하여야 합니다.
			*/
			virtual EAxlTriggerSIOCN2CHGetFunction GetStartPosition(_Out_ double* pParam);
			virtual EAxlTriggerSIOCN2CHSetFunction SetStartPosition(_In_ double dblParam);
			
			/**
			* 카운터 모듈의 트리거 출력 범위 중 상한 값을 설정한다.
			* 위치 주기 트리거 제품의 경우 위치 주기로 트리거 출력을 발생시킬 범위 중 상한 값을 설정한다.
			* 절대 위치 트리거 제품의 경우 트리거 정보가 설정된 Ram 의 마지막 번지의 트리거 정보가 적용되는 위치로 사용된다.
			* 단위 : AxcMotSetMoveUnitPerPulse로 설정한 단위.
			* Note : 상한값은 하한값보다 큰값을 설정하여야 합니다.
			*/
			virtual EAxlTriggerSIOCN2CHGetFunction GetEndPosition(_Out_ double* pParam);
			virtual EAxlTriggerSIOCN2CHSetFunction SetEndPosition(_In_ double dblParam);
			
			/**
			* 카운터 모듈의 위치 주기 모드 트리거에 사용되는 위치 주기를 설정한다.
			* 단위 : AxcMotSetMoveUnitPerPulse로 설정한 단위.
			*/
			virtual EAxlTriggerSIOCN2CHGetFunction GetTriggerPeriod(_Out_ double* pParam);
			virtual EAxlTriggerSIOCN2CHSetFunction SetTriggerPeriod(_In_ double dblParam);

			/**
			* 1. AXT_SIO_CN2CH인 경우
			* dwTrigNum	--> 131072(=0x20000) 개까지 설정 가능
			* dwTrigPos	--> DWORD형 Data 입력 가능
			* dwDirection	--> 0x0(default) : dwTrigPos[0], dwTrigPos[1] ..., dwTrigPos[dwTrigNum - 1] 순으로 Data를 Write 한다.
			* 				0x1			 : dwTrigPos[dwTrigNum - 1], dwTrigPos[dwTrigNum - 2], ..., dwTrigPos[0] 순으로 Data를 Write 한다.
			* *참고* 1) dwDirection: Data Write 순서만 다를 뿐 기능상의 차이 없음
			* 	  2) AXC Manual의 AxcTriggerSetAbs - Description을 참고하여 data를 가공 후 사용해야 함
			*/
			virtual bool ClearAbsolutePosition();
			virtual bool SetAbsoluteTriggerPosition(_In_ std::vector<DWORD>& refvctPositions);
			virtual bool SetAbsoluteTriggerPosition(_In_ std::vector<DWORD>* pvctPositions);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;
			
			/**
			* 디바이스 초기화 시 하드웨어 초기화를 켜고 끕니다.
			*/
			virtual EGetFunction GetOpenNoHardware(_Out_ bool * pParam);
			virtual ESetFunction SetOpenNoHardware(_In_ bool bParam);

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

		protected:
			bool m_bTriggerEnabled = false;

			std::vector<DWORD> m_vctTrigPos;
		};
	}
}

#endif