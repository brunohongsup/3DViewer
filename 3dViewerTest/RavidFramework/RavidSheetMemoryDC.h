#pragma once

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidSheetMemoryDC : public CDC
		{
		public:
			CRavidSheetMemoryDC(CDC* pDC);
			virtual ~CRavidSheetMemoryDC();

			CRavidSheetMemoryDC* operator->();

		private:
			CBitmap  m_bitmap;
			CBitmap* m_pOldBitmap = nullptr;
			CDC*     m_pDC = nullptr;
			CRect    m_cr = CRect(0, 0, 0, 0);
			bool     m_bIsMemDC = false;
		};
	}
}
