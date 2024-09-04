#pragma once
#include <afxext.h>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CRavidBitmapButton :
			public CBitmapButton
		{
			DECLARE_DYNCREATE(CRavidBitmapButton)

		public:
			CRavidBitmapButton();
			~CRavidBitmapButton();

		public:
			virtual bool LoadBitmaps(_In_ UINT nIDBitmap, _In_ UINT nIDSelectBitmap = 0, _In_ UINT nIDHoverBitmap = 0);

			DECLARE_MESSAGE_MAP()
		public:
			afx_msg void OnMouseHover(UINT nFlags, CPoint point);
			afx_msg void OnMouseLeave();
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);

		private:
			UINT m_nBitmap = 0;
			UINT m_nSelectBitmap = 0;

			UINT m_nHoverBitmap = 0;
		
			bool m_bTrackMouse = false;
		};
	}
}

