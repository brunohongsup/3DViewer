#pragma once

#include "UIConfigElement.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CUIConfigElementEx : public CUIConfigElement
		{
		public:
			RavidUseDynamicCreation(this);

			CUIConfigElementEx();
			virtual ~CUIConfigElementEx();
			CUIConfigElementEx(_In_ CUIConfigElement& uce);
			CUIConfigElementEx(_In_ CUIConfigElement* pUce);
			CUIConfigElementEx(_In_ CUIConfigElementEx& uce);
			CUIConfigElementEx(_In_ CUIConfigElementEx* pUce);
			CUIConfigElementEx(_In_ CRuntimeClass* pChildFrame, _In_ CRuntimeClass* pView, _In_ CRavidRect<double> rrPosition, _In_ CRavidRect<int> rrCoordinate, CString strViewName, _In_opt_ long nObjectID, _In_opt_ EUIConfigElementPositionUnit ePositionUnit = EUIConfigElementPositionUnit_Percentage);

			void operator=(_In_ const CUIConfigElement& uce);
			void operator=(_In_ CUIConfigElement* pUce);

			void operator=(_In_ const CUIConfigElementEx& uce);
			void operator=(_In_ CUIConfigElementEx* pUce);

			virtual bool Copy(_In_ CUIConfigElementEx& uce);
			virtual bool Copy(_In_ CUIConfigElementEx* pUce);

			bool SetChildFrameClass(_In_ CRuntimeClass* pChildFrame);
			CRuntimeClass* GetChildFrameClass();

		protected:
			CRuntimeClass* m_pChildFrame = nullptr;

		};
	}
}

