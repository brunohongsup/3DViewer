#pragma once

#include "DeviceImage.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		/**
		* 단독 카메라 계열의 기저 클래스입니다.
		*/
		class AFX_EXT_CLASS CDeviceCamera : public CDeviceImage
		{
		public:
			DECLARE_DYNAMIC(CDeviceCamera)

		public:
			CDeviceCamera();
			virtual ~CDeviceCamera();
			
			/**
			* 카메라를 사용하기 위한 초기화 작업을 실행합니다.
			* 순수 가상함수로 꼭 정의해야 합니다.
			*
			* return value : 실패하면 EDeviceInitializeResult_OK 이외의 값을 가집니다.
			*/
			virtual EDeviceInitializeResult Initialize() override = 0;
			
			/**
			* 카메라의 리소스를 반환하기 위한 해제 작업을 실행합니다.
			* 순수 가상함수로 꼭 정의해야 합니다.
			*
			* return value : 실패하면 EDeviceTerminateResult_OK 이외의 값을 가집니다.
			*/
			virtual EDeviceTerminateResult Terminate() override = 0;
			
			/**
			* 촬영합니다.
			* 순수 가상함수로 꼭 정의해야 합니다.
			*
			* return value : 실패하면 EDeviceGrabResult_OK 이외의 값을 가집니다.
			*/
			virtual EDeviceGrabResult Grab() override = 0;
			
			/**
			* 연속 촬영합니다.
			* 순수 가상함수로 꼭 정의해야 합니다.
			*
			* return value : 실패하면 EDeviceLiveResult_OK 이외의 값을 가집니다.
			*/
			virtual EDeviceLiveResult Live() override = 0;
			
			/**
			* 촬영을 중지합니다.
			* 순수 가상함수로 꼭 정의해야 합니다.
			*
			* return value : 실패하면 EDeviceStopResult_OK 이외의 값을 가집니다.
			*/
			virtual EDeviceStopResult Stop() override = 0;
			
			/**
			* 소프트웨어 트리거를 출력합니다.
			* 순수 가상함수로 꼭 정의해야 합니다.
			*
			* return value : 실패하면 EDeviceTriggerResult_OK 이외의 값을 가집니다.
			*/
			virtual EDeviceTriggerResult Trigger() override = 0;

		protected:
			
			/**
			* UI 컨트롤을 추가하는 역할을 합니다.
			*
			* return value : 실패하면 false 값을 가집니다.
			*/
			virtual bool AddControls();
		};
	}
}

