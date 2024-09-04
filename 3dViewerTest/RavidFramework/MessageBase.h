#pragma once

#include "RavidObjectEx.h"

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CMessageBase : public CRavidObjectEx
		{
		public:
			RavidUseDynamicCreation(this);

			CMessageBase();
			CMessageBase(_In_ CMessageBase& mb);
			CMessageBase(_In_ CMessageBase* pMB);
			CMessageBase(_In_ int nMessage, _In_ ptrdiff_t ptdParams);
			virtual ~CMessageBase();

			bool Copy(_In_ CMessageBase& mb);
			bool Copy(_In_ CMessageBase* pMB);

			virtual void SetMessage(_In_ int nMessage);
			virtual int GetMessage();

			virtual void SetParams(_In_ ptrdiff_t ptdParams);
			virtual ptrdiff_t GetParams();

		protected:
			int m_nMessage = 0;
			ptrdiff_t m_pdtParams = 0;

		};
	}
}


