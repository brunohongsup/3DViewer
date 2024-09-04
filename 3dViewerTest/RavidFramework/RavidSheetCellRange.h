#pragma once

#include "RavidSheetCellID.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidSheetCellRange
		{
		public:
			CRavidSheetCellRange(_In_opt_ __int64 nMinRow = -1, _In_opt_ __int64 nMinCol = -1, _In_opt_ __int64 nMaxRow = -1, _In_opt_ __int64 nMaxCol = -1);

			void Set(_In_opt_ __int64 nMinRow = -1, _In_opt_ __int64 nMinCol = -1, _In_opt_ __int64 nMaxRow = -1, _In_opt_ __int64 nMaxCol = -1);

			bool  IsValid();
			bool  InRange(_In_ __int64 nRow, _In_ __int64 nCol);
			bool  InRange(_In_ CRavidSheetCellID& rsci);
			__int64  Count();

			CRavidSheetCellID  GetTopLeft();
			CRavidSheetCellRange  Intersect(_In_ CRavidSheetCellRange& rscr);

			__int64 GetMinRow();
			void SetMinRow(_In_ __int64 nMinRow);

			__int64 GetMinCol();
			void SetMinCol(_In_ __int64 nMinCol);

			__int64 GetMaxRow();
			void SetMaxRow(_In_ __int64 nMaxRow);

			__int64 GetMaxCol();
			void SetMaxCol(_In_ __int64 nMaxCol);

			__int64 GetRowSpan();
			__int64 GetColSpan();

			void operator=(_In_ CRavidSheetCellRange& rscr);
			int  operator==(_In_ CRavidSheetCellRange& rscr);
			int  operator!=(_In_ CRavidSheetCellRange& rscr);

			__int64 m_nMinRow = -1;
			__int64 m_nMinCol = -1;
			__int64 m_nMaxRow = -1;
			__int64 m_nMaxCol = -1;
		};
	}
}
