#pragma once

#include "ViewObject.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CTeachingObject : public CViewObject
		{
			RavidUseDynamicCreation(this);

		public:
			CTeachingObject();
			CTeachingObject(CTeachingObject& rto);
			CTeachingObject(CTeachingObject* pRto);

			virtual ~CTeachingObject();

			virtual CString GetTitle() override;


			virtual bool Add(_In_ CRavidGeometry& geometry) override;
			virtual bool Add(_In_ CRavidGeometry* pGeometry) override;

			virtual EViewObjectContextMenuType OnContextMenu(_In_ CRavidPoint<double> rp, _In_ double dblScale) override;

		protected:

			bool HitRectTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale);
			bool HitRectTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale);

			bool IsShapeValid();
		};
	}
}