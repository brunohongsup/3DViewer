#pragma once

#include "RavidBehavior.h"

namespace Ravid
{
	namespace Framework
	{
		class CRavidImageBehavior : public CRavidBehavior
		{
		public:
			CRavidImageBehavior();
			virtual ~CRavidImageBehavior();

			virtual bool Copy(_In_ CRavidImageBehavior* pBehavior);

			virtual bool SaveImageBehavior(_In_ CImageProcessingToolImageInfo* pImageInfo);

			virtual bool Undo();
			virtual bool Redo();

			virtual CRavidObjectEx* GetObject();

		protected:
			CImageProcessingToolImageInfo* m_pObject = nullptr;

			Algorithms::CRavidImage* m_pImage = nullptr;

			CString m_strTitle = _T("");
		};
	}
}

