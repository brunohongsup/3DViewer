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
		* 프레임그레버 디바이스 계열의 기저 클래스입니다.
		*/
		class AFX_EXT_CLASS CDeviceFrameGrabber : public CDeviceImage
		{
		public:
			CDeviceFrameGrabber();
			virtual ~CDeviceFrameGrabber();

			virtual EDeviceInitializeResult Initialize() override = 0;
			virtual EDeviceTerminateResult Terminate() override = 0;

			virtual EDeviceGrabResult Grab() override = 0;
			virtual EDeviceLiveResult Live() override = 0;
			virtual EDeviceStopResult Stop() override = 0;

		protected:

			virtual bool AddControls();
		};
	}
}

