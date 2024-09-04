#pragma once

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidSheetCellID
		{
		public:
			CRavidSheetCellID(_In_ __int64 nRow = -1, _In_ __int64 nCol = -1);

			bool IsValid();
			bool operator==(_In_ CRavidSheetCellID& rsci);
			bool operator!=(_In_ CRavidSheetCellID& rsci);

		public:
			__int64 m_nRow = -1;
			__int64 m_nCol = -1;
		};
	}
}