#pragma once

#include "ViewObjectElement.h"

namespace Ravid
{
	namespace Framework
	{
		class CRavidImageViewLayer;

		class AFX_EXT_CLASS CTeachingObjectElement : public CViewObjectElement
		{
			RavidUseDynamicCreation(this);

		public:
			CTeachingObjectElement();
			CTeachingObjectElement(CTeachingObjectElement& rtoe);
			CTeachingObjectElement(CTeachingObjectElement* pRtoe);

			virtual ~CTeachingObjectElement();

			virtual bool DrawLayer(_In_ CRavidImageViewLayer* pLayer, _In_ CRavidPoint<double>& rp, _In_ double& dblScale, _In_opt_ double dblAccuracyX = 1., _In_opt_ double dblAccuracyY = 1., _In_opt_ ERavidAccuracyUnit eAccuracyUnit = ERavidAccuracyUnit_mm) override;

			virtual CRavidRect<double> GetRect() override;
			virtual bool SetRect(_In_ CRavidRect<double>* pRR) override;

			virtual CRavidPoint<double> GetDrawPoint() override;
			virtual CRavidPoint<double> GetDrawPreviewPoint() override;

			CRavidPoint<double> __InternalCalc2(CRavidQuadrangle<double> rqs, CRavidQuadrangle<double> rqt, double dblX, double dblY);
		};
	}
}