#pragma once

#include "ViewObjectElement.h"

namespace Ravid
{
	namespace Framework
	{
		class CRavidImageViewLayer;

		class AFX_EXT_CLASS CMultiSelectionObjectElement : public CViewObjectElement
		{
			RavidUseDynamicCreation(this);

		public:
			CMultiSelectionObjectElement();
			CMultiSelectionObjectElement(CMultiSelectionObjectElement& rtoe);
			CMultiSelectionObjectElement(CMultiSelectionObjectElement* pRtoe);

			virtual ~CMultiSelectionObjectElement();

			bool Copy(CMultiSelectionObjectElement& oe);
			bool Copy(CMultiSelectionObjectElement* pOE);

			virtual void Clear() override;

			virtual bool Set(CRavidGeometry* pRG) override;

			virtual bool Set(std::vector<CViewObjectElement*>& vctSelectedObjectElements);

			virtual bool DrawLayer(_In_ CRavidImageViewLayer* pLayer, _In_ CRavidPoint<double>& rp, _In_ double& dblScale, _In_opt_ double dblAccuracyX = 1., _In_opt_ double dblAccuracyY = 1., _In_opt_ ERavidAccuracyUnit eAccuracyUnit = ERavidAccuracyUnit_mm) override;

			virtual CRavidRect<double> GetRect() override;

			virtual EViewObjectContextMenuType OnContextMenu(_In_ CRavidPoint<double> rp, _In_ double dblScale) override;

			virtual bool RegisterGripInfo() override;

			virtual bool HitRectTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale);
			virtual bool HitRectTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale);

			virtual bool DeleteSelectedObjectElement(_In_ CViewObjectElement* pViewObjectElement);
			virtual int GetSelectedObjectElementCount();

			virtual bool RegisterSelectedObjectElementColor();
			virtual bool UnregisterSelectedObjectElementColor();

			virtual CViewObjectElement* GetSelectedObjectElement(int nNumber);

		protected:
			virtual bool UpdateShape();

			virtual bool InternalSetCorner(_In_ EViewObjectGripType eRTOGT, _In_ CRavidPoint<double> rpCur) override;
			virtual bool InternalSetLine(_In_ EViewObjectGripType eRTOGT, _In_ CRavidPoint<double> rpCur) override;
			virtual bool InternalSetRotate(_In_ CRavidPoint<double> rpCur) override;
			virtual bool InternalSetOffset(_In_ CRavidPoint<double> rpCur) override;

			bool m_bIsIntegerSelectedObject = false;
			std::vector<CViewObjectElement*> m_vctSelectedObjectElements;
		};
	}
}