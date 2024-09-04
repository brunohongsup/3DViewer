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
		class AFX_EXT_CLASS CDeviceEuresysDominoHarmony : public CDeviceEuresysDomino
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceEuresysDominoHarmony);

			DECLARE_DYNAMIC(CDeviceEuresysDominoHarmony)

		public:
			CDeviceEuresysDominoHarmony();
			virtual ~CDeviceEuresysDominoHarmony();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EEuresysGetFunction GetTopology(_Out_ EDeviceDominoHarmonyTopology* pParam);
			virtual EEuresysSetFunction SetTopology(_In_ EDeviceDominoHarmonyTopology eParam);

			virtual EEuresysGetFunction GetConnector(_Out_ EDeviceDominoHarmonyConnector* pParam);
			virtual EEuresysSetFunction SetConnector(_In_ EDeviceDominoHarmonyConnector eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()
		};
	}
}