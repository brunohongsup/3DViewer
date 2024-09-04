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
		class AFX_EXT_CLASS CDeviceEuresysDominoMelody : public CDeviceEuresysDomino
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceEuresysDominoMelody);

			DECLARE_DYNAMIC(CDeviceEuresysDominoMelody)

		public:
			CDeviceEuresysDominoMelody();
			virtual ~CDeviceEuresysDominoMelody();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EEuresysGetFunction GetTopology(_Out_ EDeviceDominoMelodyTopology* pParam);
			virtual EEuresysSetFunction SetTopology(_In_ EDeviceDominoMelodyTopology eParam);

			virtual EEuresysGetFunction GetConnector(_Out_ EDeviceDominoMelodyConnector* pParam);
			virtual EEuresysSetFunction SetConnector(_In_ EDeviceDominoMelodyConnector eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()
		};
	}
}