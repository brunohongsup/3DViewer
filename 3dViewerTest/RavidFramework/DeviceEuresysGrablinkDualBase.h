#pragma once

#include "DeviceEuresysGrablink.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceEuresysGrablinkDualBase : public CDeviceEuresysGrablink
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceEuresysGrablinkDualBase);

			DECLARE_DYNAMIC(CDeviceEuresysGrablinkDualBase)

		public:
			CDeviceEuresysGrablinkDualBase();
			virtual ~CDeviceEuresysGrablinkDualBase();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EEuresysGetFunction GetTopology(_Out_ EDeviceEuresysDualbaseTopology* pParam);
			virtual EEuresysSetFunction SetTopology(_In_ EDeviceEuresysDualbaseTopology eParam);

			virtual EEuresysGetFunction GetConnector(_Out_ EDeviceEuresysDualbaseConnector* pParam);
			virtual EEuresysSetFunction SetConnector(_In_ EDeviceEuresysDualbaseConnector eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()
		};
	}
}

