#pragma once

#include "RavidBehavior.h"

namespace Ravid
{
	namespace Framework
	{
		class CRavidDialogBase;
		class CImageProcessingToolInfoBase;
		class CRavidDialogBehavior : public CRavidBehavior
		{
		public:
			CRavidDialogBehavior();
			virtual ~CRavidDialogBehavior();

			virtual bool Copy(_In_ CRavidDialogBehavior* pBehavior);

			virtual bool SaveDialogBehavior(_In_ CRavidDialogBase* pDialog);

			virtual void SaveParamValue(_In_ int nParameter, _In_ CString strValue);
			virtual void SaveParamValue(_In_ CString strParameter, _In_ CString strValue);

			virtual bool Undo();
			virtual bool Redo();

			virtual CRavidObjectEx* GetObject();

		protected:
			CRavidDialogBase* m_pObject = nullptr;

			CImageProcessingToolInfoBase* m_pSrcInfo = nullptr;
			CImageProcessingToolInfoBase* m_pDstInfo = nullptr;
			CImageProcessingToolInfoBase* m_pOptInfo = nullptr;

			std::vector<std::pair<CString, CString>> m_vctParamValue;

			int m_nParameter = -1;
			CString m_strParameter = _T("");
			CString m_strValue = _T("");

			HTREEITEM m_hInsertAfter = nullptr;
		};
	}
}

