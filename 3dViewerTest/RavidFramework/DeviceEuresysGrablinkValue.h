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
		class AFX_EXT_CLASS CDeviceEuresysGrablinkValue : public CDeviceEuresysGrablink
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceEuresysGrablinkValue);

			DECLARE_DYNAMIC(CDeviceEuresysGrablinkValue)

		public:
			CDeviceEuresysGrablinkValue();
			virtual ~CDeviceEuresysGrablinkValue();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EEuresysGetFunction GetTopology(_Out_ EDeviceEuresysValueTopology* pParam);
			virtual EEuresysSetFunction SetTopology(_In_ EDeviceEuresysValueTopology eParam);

			virtual EEuresysGetFunction GetConnector(_Out_ EDeviceEuresysValueConnector* pParam);
			virtual EEuresysSetFunction SetConnector(_In_ EDeviceEuresysValueConnector eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()
		};
	}
}

