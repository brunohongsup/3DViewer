#pragma once

#include "DeviceCamera.h"

class PvDevice;
class PvStream;
class PvPipeline;

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
		class CRavidImage3DProfilePhotonFocus;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDevicePhotonfocus3D : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDevicePhotonfocus3D);

			DECLARE_DYNAMIC(CDevicePhotonfocus3D)

		public:
			CDevicePhotonfocus3D();
			virtual ~CDevicePhotonfocus3D();

			virtual EDeviceInitializeResult Initialize() override;

			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

			virtual EPhotonfocus3DGetFunction GetGrabCount(_Out_ long long* pParam);
			virtual EPhotonfocus3DSetFunction SetGrabCount(_In_ long long llParam);

			virtual EPhotonfocus3DGetFunction GetViewerType(_Out_ EDevicePhotonfocus3DViewerType* pParam);
			virtual EPhotonfocus3DSetFunction SetViewerType(_In_ EDevicePhotonfocus3DViewerType eParam);

			virtual EPhotonfocus3DGetFunction Get3DImageHeight(_Out_ long long* pParam);
			virtual EPhotonfocus3DSetFunction Set3DImageHeight(_In_ long long llParam);

			virtual EPhotonfocus3DGetFunction GetCanvasWidth(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetCanvasWidth(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetCanvasHeight(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetCanvasHeight(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetOffsetX(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetOffsetX(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetOffsetY(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetOffsetY(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetBufferCount(_Out_ int* pParam);									 
			virtual EPhotonfocus3DSetFunction SetBufferCount(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetGrabWaitingTime(_Out_ unsigned long long* pParam);
			virtual EPhotonfocus3DSetFunction SetGrabWaitingTime(_In_ unsigned long long ullParam);

			virtual EPhotonfocus3DGetFunction GetDetectTimeout(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetDetectTimeout(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetAcquisitionMode(_Out_ EDevicePhotonfocus3DAcquisitionMode* pParam);
			virtual EPhotonfocus3DSetFunction SetAcquisitionMode(_In_ EDevicePhotonfocus3DAcquisitionMode eParam);

			virtual EPhotonfocus3DGetFunction GetAcquisitionFrameCount(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetAcquisitionFrameCount(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetTriggerSelector(_Out_ EDevicePhotonfocus3DTriggerSelector* pParam);
			virtual EPhotonfocus3DSetFunction SetTriggerSelector(_In_ EDevicePhotonfocus3DTriggerSelector eParam);

			virtual EPhotonfocus3DGetFunction GetTriggerMode(_Out_ EDevicePhotonfocus3DSwitch* pParam);
			virtual EPhotonfocus3DSetFunction SetTriggerMode(_In_ EDevicePhotonfocus3DSwitch eParam);

			virtual EPhotonfocus3DGetFunction GetTriggerSource(_Out_ EDevicePhotonfocus3DTriggerSource* pParam);
			virtual EPhotonfocus3DSetFunction SetTriggerSource(_In_ EDevicePhotonfocus3DTriggerSource eParam);

			virtual EPhotonfocus3DGetFunction GetTriggerActivation(_Out_ EDevicePhotonfocus3DTriggerActivation* pParam);
			virtual EPhotonfocus3DSetFunction SetTriggerActivation(_In_ EDevicePhotonfocus3DTriggerActivation eParam);

			virtual EPhotonfocus3DGetFunction GetTriggerDelay(_Out_ double* pParam);
			virtual EPhotonfocus3DSetFunction SetTriggerDelay(_In_ double dblParam);

			virtual EPhotonfocus3DGetFunction GetTriggerDivider(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetTriggerDivider(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetABTriggerDebounce(_Out_ EDevicePhotonfocus3DFlag* pParam);
			virtual EPhotonfocus3DSetFunction SetABTriggerDebounce(_In_ EDevicePhotonfocus3DFlag eParam);

			virtual EPhotonfocus3DGetFunction GetABTriggerMode(_Out_ EDevicePhotonfocus3DABTriggerMode* pParam);
			virtual EPhotonfocus3DSetFunction SetABTriggerMode(_In_ EDevicePhotonfocus3DABTriggerMode eParam);

			virtual EPhotonfocus3DGetFunction GetABTriggerDirection(_Out_ EDevicePhotonfocus3DABTriggerDirection* pParam);
			virtual EPhotonfocus3DSetFunction SetABTriggerDirection(_In_ EDevicePhotonfocus3DABTriggerDirection eParam);

			virtual EPhotonfocus3DGetFunction GetABTriggerDivider(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetABTriggerDivider(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetABEncoderPosition(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetABEncoderPosition(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetExposureMode(_Out_ EDevicePhotonfocus3DExposureMode* pParam);
			virtual EPhotonfocus3DSetFunction SetExposureMode(_In_ EDevicePhotonfocus3DExposureMode eParam);

			virtual EPhotonfocus3DGetFunction GetExposureTime(_Out_ double* pParam);
			virtual EPhotonfocus3DSetFunction SetExposureTime(_In_ double dblParam);

			virtual EPhotonfocus3DGetFunction GetGainSelector(_Out_ EDevicePhotonfocus3DGainSelector* pParam);
			virtual EPhotonfocus3DSetFunction SetGainSelector(_In_ EDevicePhotonfocus3DGainSelector eParam);

			virtual EPhotonfocus3DGetFunction GetGain(_Out_ double* pParam);
			virtual EPhotonfocus3DSetFunction SetGain(_In_ double dblParam);

			virtual EPhotonfocus3DGetFunction GetBlackLevelSelector(_Out_ EDevicePhotonfocus3DBlackLevelSelector* pParam);
			virtual EPhotonfocus3DSetFunction SetBlackLevelSelector(_In_ EDevicePhotonfocus3DBlackLevelSelector eParam);

			virtual EPhotonfocus3DGetFunction GetBlackLevel(_Out_ double* pParam);
			virtual EPhotonfocus3DSetFunction SetBlackLevel(_In_ double dblParam);

			virtual EPhotonfocus3DGetFunction GetDigitalOffset(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetDigitalOffset(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_Threshold(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_Threshold(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_3DY(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_3DY(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_3DH(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_3DH(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_Mirror(_Out_ EDevicePhotonfocus3DFlag* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_Mirror(_In_ EDevicePhotonfocus3DFlag eParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_2DY(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_2DY(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_Gain(_Out_ double* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_Gain(_In_ double dblParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_DigitalOffset(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_DigitalOffset(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_EnPeakFilter(_Out_ EDevicePhotonfocus3DFlag* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_EnPeakFilter(_In_ EDevicePhotonfocus3DFlag eParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_PeakFilterHeightMin(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_PeakFilterHeightMin(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_PeakFilterHeightMax(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_PeakFilterHeightMax(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_PeakFilterWidthMin(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_PeakFilterWidthMin(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_PeakFilterWidthMax(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_PeakFilterWidthMax(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_EnMovingROI(_Out_ EDevicePhotonfocus3DFlag* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_EnMovingROI(_In_ EDevicePhotonfocus3DFlag eParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_MovingROIEnLaserFinder(_Out_ EDevicePhotonfocus3DFlag* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_MovingROIEnLaserFinder(_In_ EDevicePhotonfocus3DFlag eParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_MovingROIRangeY(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_MovingROIRangeY(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_MovingROIRangeH(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_MovingROIRangeH(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_MovingROIMinCol(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_MovingROIMinCol(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_MovingROIMinDistance(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_MovingROIMinDistance(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_MovingROISmoothing(_Out_ EDevicePhotonfocus3DMovingROISmoothing* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_MovingROISmoothing(_In_ EDevicePhotonfocus3DMovingROISmoothing eParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_EnAbsCoordinate(_Out_ EDevicePhotonfocus3DFlag* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_EnAbsCoordinate(_In_ EDevicePhotonfocus3DFlag eParam);

			virtual EPhotonfocus3DGetFunction GetPeak0_AbsCoordinateBase(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak0_AbsCoordinateBase(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_Threshold(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_Threshold(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_3DY(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_3DY(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_3DH(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_3DH(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_Mirror(_Out_ EDevicePhotonfocus3DFlag* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_Mirror(_In_ EDevicePhotonfocus3DFlag eParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_2DY(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_2DY(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_Gain(_Out_ double* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_Gain(_In_ double dblParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_DigitalOffset(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_DigitalOffset(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_EnPeakFilter(_Out_ EDevicePhotonfocus3DFlag* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_EnPeakFilter(_In_ EDevicePhotonfocus3DFlag eParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_PeakFilterHeightMin(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_PeakFilterHeightMin(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_PeakFilterHeightMax(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_PeakFilterHeightMax(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_PeakFilterWidthMin(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_PeakFilterWidthMin(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_PeakFilterWidthMax(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_PeakFilterWidthMax(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_EnAbsCoordinate(_Out_ EDevicePhotonfocus3DFlag* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_EnAbsCoordinate(_In_ EDevicePhotonfocus3DFlag eParam);

			virtual EPhotonfocus3DGetFunction GetPeak1_AbsCoordinateBase(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeak1_AbsCoordinateBase(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeakDetector_NrOfPeaks(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeakDetector_NrOfPeaks(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeakDetector_DataFormat3D(_Out_ int* pParam);
			virtual EPhotonfocus3DSetFunction SetPeakDetector_DataFormat3D(_In_ int nParam);

			virtual EPhotonfocus3DGetFunction GetPeakDetector_HighSpeed(_Out_ EDevicePhotonfocus3DFlag* pParam);
			virtual EPhotonfocus3DSetFunction SetPeakDetector_HighSpeed(_In_ EDevicePhotonfocus3DFlag eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

			static UINT LiveThread(_In_ LPVOID pParam);

		protected:
			Ravid::Algorithms::CRavidImage3DProfilePhotonFocus* m_pImage = nullptr;

			volatile long long m_llGrabCount;

			int m_nLastOffsetX;
			int m_nLastOffsetY;

			volatile int32_t m_nWaitTime = 0;

			CWinThread* m_pLiveThread = nullptr;

			PvDevice* m_pCamera;
			PvStream* m_pStream;
			PvPipeline* m_pPipeline;
		};
	}
}
