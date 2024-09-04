#pragma once

#include "RavidObjectEx.h"

namespace Ravid
{
	namespace Framework
	{
		class CRavidBehaviorNode;
		class CRavidBehavior : public CRavidObjectEx
		{
		public:
			CRavidBehavior();
			virtual ~CRavidBehavior();

			virtual bool Copy(_In_ CRavidBehavior* pBehavior);

			virtual bool Undo();
			virtual bool Redo();

			virtual bool AddCategory(_In_ CRavidBehavior* pBehavior);

			virtual int GetCategoryCount();
			virtual CRavidBehavior* GetCategory(_In_ int nIdx);

			virtual bool DeleteCategory(_In_ int nIdx);

			virtual CRavidObjectEx* GetObject();

		protected:
			std::vector<CRavidBehavior*> m_vctCategory;
		};
	}
}

