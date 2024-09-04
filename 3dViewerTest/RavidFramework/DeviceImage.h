#pragma once

#include "DeviceBase.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Framework
	{
		class CRavidImageView;
	}


	namespace Device
	{
		/**
		* 이미지를 사용하는 디바이스 계열의 기저 클래스입니다.
		*/
		class AFX_EXT_CLASS CDeviceImage : public CDeviceBase
		{
		public:
			DECLARE_DYNAMIC(CDeviceImage)

		public:
			CDeviceImage();
			virtual ~CDeviceImage();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual EDeviceInitializeResult Initialize() override = 0;
			virtual EDeviceTerminateResult Terminate() override = 0;

			virtual bool IsLive();
			virtual bool IsGrabAvailable();
			virtual bool IsFramegrabber();

			virtual EDeviceGrabResult Grab() = 0;
			virtual EDeviceLiveResult Live() = 0;
			virtual EDeviceStopResult Stop() = 0;

			virtual EDeviceTriggerResult Trigger() = 0;

			virtual Algorithms::CRavidImage* GetImageInfo();
			virtual Algorithms::CRavidImage* GetImageTempInfo();

			virtual Framework::CRavidImageView* GetImageView();

			virtual bool SetImageView(_In_ const std::type_info& typeInfo, _In_ int nViewID);

		public:
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();
			afx_msg void OnTimer(UINT_PTR nIDEvent);

		protected:
			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

			bool m_bIsFramegrabber = false;
			volatile bool m_bIsLive = false;
			volatile bool m_bIsGrabAvailable = true;

			int m_nImageViewID = -1;
			std::type_info* m_pTypeInfo = nullptr;

			Ravid::Algorithms::CRavidImage* m_pImageTempInfo = nullptr;
			Ravid::Algorithms::CRavidImage* m_pImageInfo = nullptr;
		};
	}
}

