#pragma once

#include "RavidBehavior.h"
#include "../RavidCore/RavidPoint.h"
#include "../RavidCore/RavidGeometry.h"

namespace Ravid
{
	namespace Framework
	{
		class CViewObjectElement;
		class CRavidObjectBehavior : public CRavidBehavior
		{
		public:
			CRavidObjectBehavior();
			virtual ~CRavidObjectBehavior();

			virtual bool Copy(_In_ CRavidObjectBehavior* pBehavior);

			virtual bool SaveObjectBehavior(_In_ CViewObjectElement* pViewObjectElement);

			virtual bool Undo();
			virtual bool Redo();

			virtual CRavidObjectEx* GetObject();

		protected:
			CViewObjectElement* m_pObject = nullptr;

			CRavidGeometry* m_pGeometry = nullptr;

			CRavidPoint<double> m_rpTextDrawPoint;

			CString m_strTitle = _T("");

			HTREEITEM m_hInsertAfter = nullptr;
		};
	}
}

