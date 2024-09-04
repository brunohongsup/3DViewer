#pragma once

#include "DeviceEuresysDomino.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceEuresysDominoSymphony : public CDeviceEuresysDomino
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceEuresysDominoSymphony);

			DECLARE_DYNAMIC(CDeviceEuresysDominoSymphony)

		public:
			CDeviceEuresysDominoSymphony();
			virtual ~CDeviceEuresysDominoSymphony();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EEuresysGetFunction GetTopology(_Out_ EDeviceDominoSymphonyTopology* pParam);
			virtual EEuresysSetFunction SetTopology(_In_ EDeviceDominoSymphonyTopology eParam);

			virtual EEuresysGetFunction GetConnector(_Out_ EDeviceDominoSymphonyConnector* pParam);
			virtual EEuresysSetFunction SetConnector(_In_ EDeviceDominoSymphonyConnector eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()
		};
	}
}