#pragma once

#include "RavidSheetCell.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidSheetCellNumeric : public CRavidSheetCell
		{
			DECLARE_DYNCREATE(CRavidSheetCellNumeric)

		public:
			virtual bool Edit(_In_ __int64 nRow, _In_ __int64 nCol, _In_ CRect rect, _In_ CPoint point, _In_ UINT nID, _In_ UINT nChar);
			virtual void EndEdit();
		};
	}
}
