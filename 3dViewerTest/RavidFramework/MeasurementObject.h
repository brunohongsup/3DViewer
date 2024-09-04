#pragma once

#include "ViewObject.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CMeasurementObject : public CViewObject
		{
		public:
			RavidUseDynamicCreation(this);

			CMeasurementObject();
			CMeasurementObject(CMeasurementObject& rmo);
			CMeasurementObject(CMeasurementObject* pRMO);

			virtual ~CMeasurementObject();

			bool Add(_In_ ERavidMeasurementType eRMT);

			virtual EViewObjectContextMenuType OnContextMenu(_In_ CRavidPoint<double> rp, _In_ double dblScale) override;

		protected:

			bool HitTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale);
			bool HitTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale);
		};
	}
}