#pragma once

#include "RavidObjectEx.h"

#include <vector>

namespace Ravid
{
	namespace Framework
	{
		class CUIConfigElement;

		class AFX_EXT_CLASS CUIConfig : public CRavidObjectEx
		{
			RavidUseDynamicCreation(this)
		public:

			CUIConfig();
			CUIConfig(_In_ CUIConfig& uce);
			CUIConfig(_In_ CUIConfig* pUce);
			virtual ~CUIConfig();

			virtual bool Copy(_In_ CUIConfig& uce);
			virtual bool Copy(_In_ CUIConfig* pUce);

			void SetTitle(_In_ CString strTitle);
			CString GetTitle();

			bool AddElement(_In_ CUIConfigElement& uce);
			bool AddElement(_In_ CUIConfigElement* pUce);

			bool DeleteElement(_In_ CUIConfigElement* pUce);

			void ClearElement();

			CUIConfigElement* GetElement(_In_ int nIndex);
			size_t GetElementCount();

			bool GetElementsObjectID(_Out_ std::vector<long>& vctRegisterSequenceNumbers);

		private:
			CString m_strTitle = _T("");
			std::vector<CUIConfigElement*> m_vctUIConfigElements;
		};
	}
}

