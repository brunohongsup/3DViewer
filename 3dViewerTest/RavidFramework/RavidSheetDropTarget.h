#pragma once

#include <afxole.h>

namespace Ravid
{
	namespace Framework
	{
		class CRavidSheet;

		class AFX_EXT_CLASS CRavidSheetDropTarget : public COleDropTarget
		{
		public:
			CRavidSheetDropTarget();
			virtual ~CRavidSheetDropTarget();

		public:
			bool Register(_In_ CRavidSheet* pSheetCtrl);
			virtual void Revoke() override;

			virtual BOOL        OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) override;
			virtual DROPEFFECT  OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) override;
			virtual void        OnDragLeave(CWnd* pWnd) override;
			virtual DROPEFFECT  OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) override;
			virtual DROPEFFECT  OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point) override;

		protected:
			DECLARE_MESSAGE_MAP()

		protected:
			CRavidSheet* m_pSheetCtrl = nullptr;
			bool       m_bIsRegistered = false;
		};
	}
}