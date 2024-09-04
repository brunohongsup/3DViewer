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
		class AFX_EXT_CLASS CDeviceEuresysGrablinkFull : public CDeviceEuresysGrablink
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceEuresysGrablinkFull);

			DECLARE_DYNAMIC(CDeviceEuresysGrablinkFull)

		public:
			CDeviceEuresysGrablinkFull();
			virtual ~CDeviceEuresysGrablinkFull();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EEuresysGetFunction GetTopology(_Out_ EDeviceEuresysFullTopology* pParam);
			virtual EEuresysSetFunction SetTopology(_In_ EDeviceEuresysFullTopology eParam);

			virtual EEuresysGetFunction GetConnector(_Out_ EDeviceEuresysFullConnector* pParam);
			virtual EEuresysSetFunction SetConnector(_In_ EDeviceEuresysFullConnector eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()
		};
	}
}