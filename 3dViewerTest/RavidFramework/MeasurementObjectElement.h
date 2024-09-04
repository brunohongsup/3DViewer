#pragma once

#include "ViewObjectElement.h"

#include <memory>

namespace Ravid
{
	namespace Framework
	{
		class CMeasurementObject;
		class CRavidImageViewLayer;

		class AFX_EXT_CLASS CMeasurementObjectElement : public CViewObjectElement
		{
			RavidUseDynamicCreation(this);

		public: 
			CMeasurementObjectElement();
			CMeasurementObjectElement(CMeasurementObjectElement& rtoe);
			CMeasurementObjectElement(CMeasurementObjectElement* pRtoe);

			virtual ~CMeasurementObjectElement();

			bool Copy(_In_ CMeasurementObjectElement& rmoe);
			bool Copy(_In_ CMeasurementObjectElement* pRMOE);

			bool Set(_In_ ERavidMeasurementType eRMT);

			virtual bool Set(_In_ EViewObjectGripType eRTOGT, _In_ CRavidPoint<double> rpCur) override;

			virtual CRavidRect<double> GetRect() override;
			virtual bool SetRect(_In_ CRavidRect<double>* pRR) override;

			virtual bool DrawLayer(_In_ CRavidImageViewLayer* pLayer, _In_ CRavidPoint<double>& rp, _In_ double& dblScale, _In_opt_ double dblAccuracyX = 1., _In_opt_ double dblAccuracyY = 1., _In_opt_ ERavidAccuracyUnit eAccuracyUnit = ERavidAccuracyUnit_mm) override;

			virtual CRavidPoint<double> GetDrawPoint() override;

			virtual bool HitTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale) override;
			virtual bool HitTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale) override;

			virtual EViewObjectGripType HitGripTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale, _Out_opt_ CRavidPoint<double>* pRPRef = nullptr) override;
			virtual EViewObjectGripType HitGripTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale, _Out_opt_ CRavidPoint<double>* pRPRef = nullptr) override;

			virtual bool HitLineTest(_In_ CRavidPoint<double>& rp, _In_ double dblScale) override;
			virtual bool HitLineTest(_In_ CRavidPoint<double>* pRP, _In_ double dblScale) override;

			virtual EViewObjectContextMenuType OnContextMenu(_In_ CRavidPoint<double> rp, _In_ double dblScale);

			void SetMeasurementType(_In_ ERavidMeasurementType eRMT);
			ERavidMeasurementType GetMeasurementType();

			virtual bool RegisterGripInfo() override;

		protected:

			bool MakeCircle(_In_ CRavidPoint<double>* pRP1, _In_ CRavidPoint<double>* pRP2, _In_ CRavidPoint<double>* pRP3, _Out_ CRavidEllipse<double>* pRE);

			bool GetNumberPoint(_In_ CRavidLine<double>* pRL, _In_ CRavidPoint<double>* pRP, _Out_ CRavidPoint<double>* pRPResult);

		protected:
			ERavidMeasurementType m_eMeasurementType = ERavidMeasurementType_None;
		};
	}
}