#pragma once
#include "RavidSheetCell.h"

#include <vector>

namespace Ravid
{
	namespace Framework
	{
		class CRavidSheetPropertyCell:
			public CRavidSheetCell
		{
		public:
			CRavidSheetPropertyCell();
			~CRavidSheetPropertyCell();

			virtual void OnEndEdit() { CRavidSheetCell::OnEndEdit(); }

			bool SetPropertyControl(SModelElementProperty* sProperty, int nRow);
			SModelElementProperty* GetPropertyControl(int nRow);
			int SizePropertyControl() { return (int)m_vctProperty.size(); }

			void ClearPropertyControl() { m_vctProperty.clear(); }

			int GetPropertySheetRowNum(SModelElementProperty* pProperty);
			std::vector<SModelElementProperty *>* GetAllProperty();

		protected:
			std::vector<SModelElementProperty *> m_vctProperty;
		};
	}
}