#pragma once

#include "DeviceMatroxIntellicam.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceMatroxSolios : public CDeviceMatroxIntellicam
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceMatroxSolios);

			DECLARE_DYNAMIC(CDeviceMatroxSolios)

		public:
			CDeviceMatroxSolios();
			virtual ~CDeviceMatroxSolios();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool MappingDcfFile();

			virtual bool MakeDcfFile(_In_ CString strPath);

			virtual bool LoadSettings() override;

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;


		public:
			DECLARE_MESSAGE_MAP()

		protected:
			std::vector<CString> m_vctMappingData;
		};
	}
}