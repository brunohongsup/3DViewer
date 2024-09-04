#pragma once

#include "ViewObject.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CMultiSelectionObject : public CViewObject
		{
			RavidUseDynamicCreation(this);

		public:
			CMultiSelectionObject();
			CMultiSelectionObject(CMultiSelectionObject& rto);
			CMultiSelectionObject(CMultiSelectionObject* pRto);

			virtual ~CMultiSelectionObject();

			virtual bool Add(_In_ std::vector<CViewObjectElement*>& vctSelectedObjectElements);

			virtual EViewObjectContextMenuType OnContextMenu(_In_ CRavidPoint<double> rp, _In_ double dblScale) override;

			virtual bool HitTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale);
			virtual bool HitTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale);

			virtual bool HitRectTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale);
			virtual bool HitRectTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale);

			virtual bool HitGripTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale);
			virtual bool HitGripTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale);

			virtual bool DeleteSelectedObjectElement(_In_ CViewObjectElement* pViewObjectElement);
			virtual int GetSelectedObjectElementCount();

			virtual bool RegisterSelectedObjectElementColor();
			virtual bool UnregisterSelectedObjectElementColor();

		protected:

			bool IsShapeValid();
		};
	}
}