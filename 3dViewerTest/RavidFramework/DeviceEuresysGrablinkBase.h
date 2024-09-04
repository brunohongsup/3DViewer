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
		class AFX_EXT_CLASS CDeviceEuresysGrablinkBase : public CDeviceEuresysGrablink
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceEuresysGrablinkBase);

			DECLARE_DYNAMIC(CDeviceEuresysGrablinkBase)

		public:
			CDeviceEuresysGrablinkBase();
			virtual ~CDeviceEuresysGrablinkBase();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EEuresysGetFunction GetTopology(_Out_ EDeviceEuresysBaseTopology* pParam);
			virtual EEuresysSetFunction SetTopology(_In_ EDeviceEuresysBaseTopology eParam);

			virtual EEuresysGetFunction GetConnector(_Out_ EDeviceEuresysBaseConnector* pParam);
			virtual EEuresysSetFunction SetConnector(_In_ EDeviceEuresysBaseConnector eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()
		};
	}
}

