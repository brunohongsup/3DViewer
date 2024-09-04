#pragma once

#include "DeviceCamera.h"

#define BGAPI2CALL __stdcall
#define BGAPI2_DECL __declspec(dllimport)

namespace BGAPI2
{
	class System;
	class SystemList;

	class Interface;
	class InterfaceList;

	class Device;
	class DeviceList;

	class DataStream;
	class DataStreamList;

	class Buffer;
	class BufferList;

	class ImageProcessor;
}

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceBaumerGigE : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceBaumerGigE);

			DECLARE_DYNAMIC(CDeviceBaumerGigE)

		public:
			CDeviceBaumerGigE();
			virtual ~CDeviceBaumerGigE();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

		public:
			virtual EBaumerGetFunction GetPixelFormat(_Out_ EDeviceBaumerPixelFormat* pParam);
			virtual EBaumerSetFunction SetPixelFormat(_In_ EDeviceBaumerPixelFormat eParam);

			virtual EBaumerGetFunction GetTriggerSelector(_Out_ EDeviceBaumerTriggerSelector* pParam);
			virtual EBaumerSetFunction SetTriggerSelector(_In_ EDeviceBaumerTriggerSelector eParam);

			virtual EBaumerGetFunction GetFrameStartMode(_Out_ EDeviceBaumerSwitch* pParam);
			virtual EBaumerSetFunction SetFrameStartMode(_In_ EDeviceBaumerSwitch eParam);

			virtual EBaumerGetFunction GetLineStartMode(_Out_ EDeviceBaumerSwitch* pParam);
			virtual EBaumerSetFunction SetLineStartMode(_In_ EDeviceBaumerSwitch eParam);

			virtual EBaumerGetFunction GetAcquisitionStartMode(_Out_ EDeviceBaumerSwitch* pParam);
			virtual EBaumerSetFunction SetAcquisitionStartMode(_In_ EDeviceBaumerSwitch eParam);

			virtual EBaumerGetFunction GetFrameStartTriggerSource(_Out_ EDeviceBaumerTriggerSource* pParam);
			virtual EBaumerSetFunction SetFrameStartTriggerSource(_In_ EDeviceBaumerTriggerSource eParam);

			virtual EBaumerGetFunction GetLineStartTriggerSource(_Out_ EDeviceBaumerTriggerSource* pParam);
			virtual EBaumerSetFunction SetLineStartTriggerSource(_In_ EDeviceBaumerTriggerSource eParam);

			virtual EBaumerGetFunction GetAcquisitionStartTriggerSource(_Out_ EDeviceBaumerTriggerSource* pParam);
			virtual EBaumerSetFunction SetAcquisitionStartTriggerSource(_In_ EDeviceBaumerTriggerSource eParam);

			virtual EBaumerGetFunction GetFrameStartTriggerActivation(_Out_ EDeviceBaumerTriggerActivation* pParam);
			virtual EBaumerSetFunction SetFrameStartTriggerActivation(_In_ EDeviceBaumerTriggerActivation eParam);

			virtual EBaumerGetFunction GetLineStartTriggerActivation(_Out_ EDeviceBaumerTriggerActivation* pParam);
			virtual EBaumerSetFunction SetLineStartTriggerActivation(_In_ EDeviceBaumerTriggerActivation eParam);

			virtual EBaumerGetFunction GetAcquisitionStartTriggerActivation(_Out_ EDeviceBaumerTriggerActivation* pParam);
			virtual EBaumerSetFunction SetAcquisitionStartTriggerActivation(_In_ EDeviceBaumerTriggerActivation eParam);

			virtual EBaumerGetFunction GetExposureAuto(_Out_ EDeviceBaumerAuto* pParam);
			virtual EBaumerSetFunction SetExposureAuto(_In_ EDeviceBaumerAuto eParam);

			virtual EBaumerGetFunction GetExposureMode(_Out_ EDeviceBaumerExposureMode* pParam);
			virtual EBaumerSetFunction SetExposureMode(_In_ EDeviceBaumerExposureMode eParam);

			virtual EBaumerGetFunction GetGainAuto(_Out_ EDeviceBaumerAuto* pParam);
			virtual EBaumerSetFunction SetGainAuto(_In_ EDeviceBaumerAuto eParam);

			virtual EBaumerGetFunction GetGainSelector(_Out_ EDeviceBaumerGainSelector* pParam);
			virtual EBaumerSetFunction SetGainSelector(_In_ EDeviceBaumerGainSelector eParam);

			virtual EBaumerGetFunction GetGrabCount(_Out_ int* pParam);
			virtual EBaumerSetFunction SetGrabCount(_In_ int nParam);

			virtual EBaumerGetFunction GetCanvasWidth(_Out_ int* pParam);
			virtual EBaumerSetFunction SetCanvasWidth(_In_ int nParam);

			virtual EBaumerGetFunction GetCanvasHeight(_Out_ int* pParam);
			virtual EBaumerSetFunction SetCanvasHeight(_In_ int nParam);

			virtual EBaumerGetFunction GetOffsetX(_Out_ int* pParam);
			virtual EBaumerSetFunction SetOffsetX(_In_ int nParam);

			virtual EBaumerGetFunction GetOffsetY(_Out_ int* pParam);
			virtual EBaumerSetFunction SetOffsetY(_In_ int nParam);

			virtual EBaumerGetFunction GetGrabWaitingTime(_Out_ int * pParam);
			virtual EBaumerSetFunction SetGrabWaitingTime(_In_ int nParam);

			virtual EBaumerGetFunction GetGrabBufferCount(_Out_ int* pParam);
			virtual EBaumerSetFunction SetGrabBufferCount(_In_ int nParam);

			virtual EBaumerGetFunction GetExposure(_Out_ int* pParam);
			virtual EBaumerSetFunction SetExposure(_In_ int nParam);

			virtual EBaumerGetFunction GetAllGain(_Out_ double* pParam);
			virtual EBaumerSetFunction SetAllGain(_In_ double dblParam);

			virtual EBaumerGetFunction GetAnalogGain(_Out_ int* pParam);
			virtual EBaumerSetFunction SetAnalogGain(_In_ int nParam);

			virtual EBaumerGetFunction GetDigitalGain(_Out_ int* pParam);
			virtual EBaumerSetFunction SetDigitalGain(_In_ int nParam);

			virtual EBaumerGetFunction GetTap1Gain(_Out_ int* pParam);
			virtual EBaumerSetFunction SetTap1Gain(_In_ int nParam);

			virtual EBaumerGetFunction GetTap2Gain(_Out_ int* pParam);
			virtual EBaumerSetFunction SetTap2Gain(_In_ int nParam);

			virtual EBaumerGetFunction GetFlipHorizontal(_Out_ bool* pParam);
			virtual EBaumerSetFunction SetFlipHorizontal(_In_ bool bParam);

			virtual EBaumerGetFunction GetFlipVertical(_Out_ bool* pParam);
			virtual EBaumerSetFunction SetFlipVertical(_In_ bool bParam);

			virtual EBaumerGetFunction GetLineSelector(_Out_ EDeviceBaumerLineSelector* pParam);
			virtual EBaumerSetFunction SetLineSelector(_In_ EDeviceBaumerLineSelector eParam);

			virtual EBaumerGetFunction GetLine0Inverter(_Out_ EDeviceBaumerSwitch* pParam);
			virtual EBaumerSetFunction SetLine0Inverter(_In_ EDeviceBaumerSwitch eParam);

			virtual EBaumerGetFunction GetLine0Mode(_Out_ EDeviceBaumerLineMode* pParam);
			virtual EBaumerSetFunction SetLine0Mode(_In_ EDeviceBaumerLineMode eParam);

			virtual EBaumerGetFunction GetLine1Inverter(_Out_ EDeviceBaumerSwitch* pParam);
			virtual EBaumerSetFunction SetLine1Inverter(_In_ EDeviceBaumerSwitch eParam);
			
			virtual EBaumerGetFunction GetLine1Mode(_Out_ EDeviceBaumerLineMode* pParam);
			virtual EBaumerSetFunction SetLine1Mode(_In_ EDeviceBaumerLineMode eParam);
			
			virtual EBaumerGetFunction GetLine1Source(_Out_ EDeviceBaumerLineSource* pParam);
			virtual EBaumerSetFunction SetLine1Source(_In_ EDeviceBaumerLineSource eParam);

			virtual EBaumerGetFunction GetLine2Inverter(_Out_ EDeviceBaumerSwitch* pParam);
			virtual EBaumerSetFunction SetLine2Inverter(_In_ EDeviceBaumerSwitch eParam);

			virtual EBaumerGetFunction GetLine2Mode(_Out_ EDeviceBaumerLineMode* pParam);
			virtual EBaumerSetFunction SetLine2Mode(_In_ EDeviceBaumerLineMode eParam);

			virtual EBaumerGetFunction GetLine2Source(_Out_ EDeviceBaumerLineSource* pParam);
			virtual EBaumerSetFunction SetLine2Source(_In_ EDeviceBaumerLineSource eParam);

			virtual EBaumerGetFunction GetLine3Inverter(_Out_ EDeviceBaumerSwitch* pParam);
			virtual EBaumerSetFunction SetLine3Inverter(_In_ EDeviceBaumerSwitch eParam);

			virtual EBaumerGetFunction GetLine3Mode(_Out_ EDeviceBaumerLineMode* pParam);
			virtual EBaumerSetFunction SetLine3Mode(_In_ EDeviceBaumerLineMode eParam);

			virtual EBaumerGetFunction GetLine3Source(_Out_ EDeviceBaumerLineSource* pParam);
			virtual EBaumerSetFunction SetLine3Source(_In_ EDeviceBaumerLineSource eParam);

			virtual EBaumerGetFunction GetUserOutputSelector(_Out_ EDeviceBaumerUserOutputSelector* pParam);
			virtual EBaumerSetFunction SetUserOutputSelector(_In_ EDeviceBaumerUserOutputSelector eParam);

			virtual EBaumerGetFunction GetUserOutputValue(_Out_ EDeviceBaumerSwitch* pParam);
			virtual EBaumerSetFunction SetUserOutputValue(_In_ EDeviceBaumerSwitch eParam);

			virtual EBaumerGetFunction GetUserOutputValueAll(_Out_ int* pParam);
			virtual EBaumerSetFunction SetUserOutputValueAll(_In_ int nParam);

			virtual EBaumerGetFunction GetTimerSelector(_Out_ EDeviceBaumerTimerSelector* pParam);
			virtual EBaumerSetFunction SetTimerSelector(_In_ EDeviceBaumerTimerSelector eParam);

			virtual EBaumerGetFunction GetTimerTriggerSource(_Out_ EDeviceBaumerTimerTriggerSource* pParam);
			virtual EBaumerSetFunction SetTimerTriggerSource(_In_ EDeviceBaumerTimerTriggerSource eParam);

			virtual EBaumerGetFunction GetTimerTriggerActivation(_Out_ EDeviceBaumerTriggerActivation* pParam);
			virtual EBaumerSetFunction SetTimerTriggerActivation(_In_ EDeviceBaumerTriggerActivation eParam);

			virtual EBaumerGetFunction GetTimerDuration(_Out_ double* pParam);
			virtual EBaumerSetFunction SetTimerDuration(_In_ double dblParam);

			virtual EBaumerGetFunction GetTimerDelay(_Out_ double* pParam);
			virtual EBaumerSetFunction SetTimerDelay(_In_ double dblParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

			static void BGAPI2CALL BufferHandler(void * callBackOwner, BGAPI2::Buffer * pBufferFilled);

			unsigned long GetFrameNumber();

			static UINT LiveThread(_In_ LPVOID pParam);

		protected:
			volatile long m_nGrabCount = 0;

			volatile int32_t m_nWaitTime = 0;

			CWinThread* m_pLiveThread = nullptr;

			BGAPI2::System* m_pSystem = nullptr;

			BGAPI2::Interface* m_pInterface = nullptr;

			BGAPI2::Device* m_pDevice = nullptr;

			BGAPI2::BufferList* m_pBufferList = nullptr;
			BGAPI2::Buffer* m_pBuffer = nullptr;

			BGAPI2::DataStreamList* m_pDataStreamList = nullptr;
			BGAPI2::DataStream* m_pDataStream = nullptr;

			static BGAPI2::ImageProcessor* m_pImgProc;

			static CCriticalSection m_csDeviceList;
		};
	}
}