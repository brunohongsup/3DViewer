#pragma once

#include "RavidObjectEx.h"

#include "../RavidCore/RavidRect.h"

namespace Ravid
{
	namespace Framework
	{
		class CTeachingObject;

		class AFX_EXT_CLASS CUIConfigElement : public CRavidObjectEx
		{
			RavidUseDynamicCreation(this);
		public:
			CUIConfigElement();
			CUIConfigElement(_In_ CUIConfigElement& uce);
			CUIConfigElement(_In_ CUIConfigElement* pUce);
			CUIConfigElement(_In_ CRuntimeClass* pView, _In_ CRavidRect<double> rrPosition, _In_ CRavidRect<int> rrCoordinate, CString strViewName, _In_opt_ long nObjectID, _In_opt_ EUIConfigElementPositionUnit ePositionUnit = EUIConfigElementPositionUnit_Percentage);
			virtual ~CUIConfigElement();

			virtual bool Copy(_In_ CUIConfigElement& uce);
			virtual bool Copy(_In_ CUIConfigElement* pUce);

			bool SetViewClass(_In_ CRuntimeClass* pView);
			CRuntimeClass* GetViewClass();

			void SetPosition(_In_ CRavidRect<double> rrPosition);
			CRavidRect<double> GetPosition();

			void SetCoordinate(_In_ CRavidRect<int> rrCoordinate);
			CRavidRect<int> GetCoordinate();

			void SetPositionUnit(_In_opt_ EUIConfigElementPositionUnit ePositionUnit = EUIConfigElementPositionUnit_Percentage);
			EUIConfigElementPositionUnit GetPositionUnit();

			void SetViewName(_In_ CString strViewName);
			CString GetViewName();

			CTeachingObject* GetTeachingObjectAt(_In_ size_t stNumber);

			size_t GetTeachingObjectCount();

			bool AddTeachingObject(_In_ CTeachingObject& rto);
			bool AddTeachingObject(_In_ CTeachingObject* pRto);

			bool DeleteTeachingObjectAt(_In_ size_t stNumber);

			void ClearTeachingObject();



			CTeachingObject* GetEditTeachingObjectAt(_In_ size_t stNumber);

			size_t GetEditTeachingObjectCount();

			bool AddEditTeachingObject(_In_ CTeachingObject& rto);
			bool AddEditTeachingObject(_In_ CTeachingObject* pRto);

			bool DeleteEditTeachingObjectAt(_In_ size_t stNumber);

			void ClearEditTeachingObject();



			CString GetInspectMenuItemAt(_In_ size_t stNumber);

			size_t GetInspectionMenuItemCount();

			bool AddInspectionMenuItem(_In_ CString strInspection);

			bool DeleteInspectionMenuItemAt(_In_ size_t stNumber);

			void ClearInspectionMenuItem();

		private:
			void NumberTeachingObject();
			void SortEditTeachingObject();

		protected:
			CRuntimeClass* m_pView = nullptr;
			CRavidRect<double> m_rrPosition;
			CRavidRect<int> m_rrCoordinate;
			EUIConfigElementPositionUnit m_ePositionUnit = EUIConfigElementPositionUnit_Percentage;
			CString m_strViewName;
			std::vector<CTeachingObject*> m_vctTeachingObjects;
			std::vector<CTeachingObject*> m_vctEditTeachingObjects;
			std::vector<CString> m_vctInspection;
		};
	}
}

