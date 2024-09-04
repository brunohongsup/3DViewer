#pragma once

#include "DeviceCamera.h"
#include "../RavidCore/RavidPoint.h"

namespace MTF_API
{
	typedef int mtfHandle;

	typedef struct _mtfDeviceInfo mtfDeviceInfo;
	typedef struct _mtfFrameInfo mtfFrameInfo;
}

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceMeereTOFGigE : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceMeereTOFGigE);

			DECLARE_DYNAMIC(CDeviceMeereTOFGigE)

		public:
			CDeviceMeereTOFGigE();
			virtual ~CDeviceMeereTOFGigE();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual EDeviceInitializeResult Initialize() override;
			
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

			virtual EMeereTOFGetFunction GetDrawPoint(bool* pParam);
			virtual EMeereTOFSetFunction SetDrawPoint(bool bParam);

			virtual EMeereTOFGetFunction GetInitType(EDeviceMeereTOFInitType* pParam);
			virtual EMeereTOFSetFunction SetInitType(EDeviceMeereTOFInitType eParam);

			virtual EMeereTOFGetFunction GetIntegrationTime(int* pParam);
			virtual EMeereTOFSetFunction SetIntegrationTime(int nParam);

			virtual EMeereTOFGetFunction GetOffset(int* pParam);
			virtual EMeereTOFSetFunction SetOffset(int nParam);

			virtual EMeereTOFGetFunction GetDepthThreshold(int* pParam);
			virtual EMeereTOFSetFunction SetDepthThreshold(int nParam);

			virtual EMeereTOFGetFunction GetScatterThreshold(int* pParam);
			virtual EMeereTOFSetFunction SetScatterThreshold(int nParam);

			virtual EMeereTOFGetFunction GetMinDepth(int* pParam);
			virtual EMeereTOFSetFunction SetMinDepth(int nParam);

			virtual EMeereTOFGetFunction GetMaxDepth(int* pParam);
			virtual EMeereTOFSetFunction SetMaxDepth(int nParam);

			virtual EMeereTOFGetFunction GetFlipHorizontal(bool* pParam);
			virtual EMeereTOFSetFunction SetFlipHorizontal(bool bParam);

			virtual EMeereTOFGetFunction GetFlipVertical(bool* pParam);
			virtual EMeereTOFSetFunction SetFlipVertical(bool bParam);

			virtual EMeereTOFGetFunction GetMultiSyncMode(bool* pParam);
			virtual EMeereTOFSetFunction SetMultiSyncMode(bool bParam);

			virtual EMeereTOFGetFunction GetFPSDelay(int* pParam);
			virtual EMeereTOFSetFunction SetFPSDelay(int nParam);

			virtual EMeereTOFGetFunction GetRemoveEdge(bool* pParam);
			virtual EMeereTOFSetFunction SetRemoveEdge(bool bParam);

			virtual EMeereTOFGetFunction GetUndistortion(EDeviceMeereTOFUndistortion* pParam);
			virtual EMeereTOFSetFunction SetUndistortion(EDeviceMeereTOFUndistortion eParam);

			virtual EMeereTOFGetFunction GetOrthogonal(bool* pParam);
			virtual EMeereTOFSetFunction SetOrthogonal(bool bParam);

			virtual EMeereTOFGetFunction GetFocalLengthX(float* pParam);
			virtual EMeereTOFSetFunction SetFocalLengthX(float fParam);

			virtual EMeereTOFGetFunction GetFocalLengthY(float* pParam);
			virtual EMeereTOFSetFunction SetFocalLengthY(float fParam);

			virtual EMeereTOFGetFunction GetPrincipalPointX(float* pParam);
			virtual EMeereTOFSetFunction SetPrincipalPointX(float fParam);

			virtual EMeereTOFGetFunction GetPrincipalPointY(float* pParam);
			virtual EMeereTOFSetFunction SetPrincipalPointY(float fParam);

			virtual EMeereTOFGetFunction GetDistortionK1(float* pParam);
			virtual EMeereTOFSetFunction SetDistortionK1(float fParam);

			virtual EMeereTOFGetFunction GetDistortionK2(float* pParam);
			virtual EMeereTOFSetFunction SetDistortionK2(float fParam);

			virtual EMeereTOFGetFunction GetDistortionK3(float* pParam);
			virtual EMeereTOFSetFunction SetDistortionK3(float fParam);

			virtual EMeereTOFGetFunction GetDistortionP1(float* pParam);
			virtual EMeereTOFSetFunction SetDistortionP1(float fParam);

			virtual EMeereTOFGetFunction GetDistortionP2(float* pParam);
			virtual EMeereTOFSetFunction SetDistortionP2(float fParam);

			virtual EMeereTOFGetFunction GetSkew(float* pParam);
			virtual EMeereTOFSetFunction SetSkew(float fParam);

			virtual EMeereTOFGetFunction GetRGBFocalLengthX(float* pParam);
			virtual EMeereTOFSetFunction SetRGBFocalLengthX(float fParam);

			virtual EMeereTOFGetFunction GetRGBFocalLengthY(float* pParam);
			virtual EMeereTOFSetFunction SetRGBFocalLengthY(float fParam);

			virtual EMeereTOFGetFunction GetRGBPrincipalPointX(float* pParam);
			virtual EMeereTOFSetFunction SetRGBPrincipalPointX(float fParam);

			virtual EMeereTOFGetFunction GetRGBPrincipalPointY(float* pParam);
			virtual EMeereTOFSetFunction SetRGBPrincipalPointY(float fParam);

			virtual EMeereTOFGetFunction GetRGBDistortionK1(float* pParam);
			virtual EMeereTOFSetFunction SetRGBDistortionK1(float fParam);

			virtual EMeereTOFGetFunction GetRGBDistortionK2(float* pParam);
			virtual EMeereTOFSetFunction SetRGBDistortionK2(float fParam);

			virtual EMeereTOFGetFunction GetRGBDistortionK3(float* pParam);
			virtual EMeereTOFSetFunction SetRGBDistortionK3(float fParam);

			virtual EMeereTOFGetFunction GetRGBDistortionP1(float* pParam);
			virtual EMeereTOFSetFunction SetRGBDistortionP1(float fParam);

			virtual EMeereTOFGetFunction GetRGBDistortionP2(float* pParam);
			virtual EMeereTOFSetFunction SetRGBDistortionP2(float fParam);

			virtual EMeereTOFGetFunction GetRGBSkew(float* pParam);
			virtual EMeereTOFSetFunction SetRGBSkew(float fParam);

			virtual EMeereTOFGetFunction GetExtrinsicR11(float* pParam);
			virtual EMeereTOFSetFunction SetExtrinsicR11(float fParam);

			virtual EMeereTOFGetFunction GetExtrinsicR12(float* pParam);
			virtual EMeereTOFSetFunction SetExtrinsicR12(float fParam);

			virtual EMeereTOFGetFunction GetExtrinsicR13(float* pParam);
			virtual EMeereTOFSetFunction SetExtrinsicR13(float fParam);

			virtual EMeereTOFGetFunction GetExtrinsicR21(float* pParam);
			virtual EMeereTOFSetFunction SetExtrinsicR21(float fParam);

			virtual EMeereTOFGetFunction GetExtrinsicR22(float* pParam);
			virtual EMeereTOFSetFunction SetExtrinsicR22(float fParam);

			virtual EMeereTOFGetFunction GetExtrinsicR23(float* pParam);
			virtual EMeereTOFSetFunction SetExtrinsicR23(float fParam);

			virtual EMeereTOFGetFunction GetExtrinsicR31(float* pParam);
			virtual EMeereTOFSetFunction SetExtrinsicR31(float fParam);

			virtual EMeereTOFGetFunction GetExtrinsicR32(float* pParam);
			virtual EMeereTOFSetFunction SetExtrinsicR32(float fParam);

			virtual EMeereTOFGetFunction GetExtrinsicR33(float* pParam);
			virtual EMeereTOFSetFunction SetExtrinsicR33(float fParam);

			virtual EMeereTOFGetFunction GetExtrinsicTx(float* pParam);
			virtual EMeereTOFSetFunction SetExtrinsicTx(float fParam);

			virtual EMeereTOFGetFunction GetExtrinsicTy(float* pParam);
			virtual EMeereTOFSetFunction SetExtrinsicTy(float fParam);

			virtual EMeereTOFGetFunction GetExtrinsicTz(float* pParam);
			virtual EMeereTOFSetFunction SetExtrinsicTz(float fParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

			virtual Ravid::Algorithms::CRavidImage* Get3DXYZData();
			virtual bool GetInformation(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ float* pXCoordinate, _Out_ float* pYCoordinate, _Out_ float* pDistance);
			virtual void DrawPixelInfo(_In_ int nDrawIndex);

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

			static UINT LiveThread(_In_ LPVOID pParam);

		protected:
			bool m_bColorCamera = false;
			
			unsigned long m_nGrabCount = 0;

			Ravid::Algorithms::CRavidImage* m_p3DXYZCoordinates = nullptr;

			MTF_API::mtfFrameInfo* m_pFrameInfo = nullptr;
			MTF_API::mtfDeviceInfo* m_pDeviceInfo = nullptr;
			MTF_API::mtfHandle m_hDevice = -1;

			bool m_bDrawPoint = false;
			CRavidPoint<int> m_DrawPoint;

			volatile int32_t m_nWaitTime = 0;

			ptrdiff_t m_nCanvasX = 0;
			ptrdiff_t m_nCanvasY = 0;

			CWinThread* m_pLiveThread = nullptr;
		};
	}
}
