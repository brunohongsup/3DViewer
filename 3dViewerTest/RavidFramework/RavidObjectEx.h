#pragma once

#include "../Libraries/Includes/RavidSecurity/RavidObject.h"
#include <functional>
#include <list>

#define RavidThreadMessageID 32548
typedef std::function<void(void)> RavidThreadMessageContext;
#define CreateRavidThreadMessageContext(context) RavidThreadMessageContext* context = new RavidThreadMessageContext; *context = [=]() -> void 
#define SendRavidThreadMessage(context) AfxGetApp()->m_pMainWnd->SendMessage(RavidThreadMessageID, 0, (LPARAM)context);
#define PostRavidThreadMessage(context) AfxGetApp()->m_pMainWnd->PostMessage(RavidThreadMessageID, 0, (LPARAM)context);

class CMultiDocTemplate;
class Ravid::Algorithms::CRavidImage;


namespace Ravid
{
	namespace Framework
	{
		class CMessageBase;

		class AFX_EXT_CLASS CRavidObjectEx : public CRavidObject
		{
		public:
			RavidUseDynamicCreation();

			CRavidObjectEx();
			virtual ~CRavidObjectEx();

			virtual ptrdiff_t OnMessage(_In_ CMessageBase* pMessage);

			bool SetReceiveMessage(_In_ bool bSet);
			bool IsReceiveMessage();

		protected:
			bool m_bIsReceiveMessage = false;

			static CCriticalSection m_csReceiveMessageObject;

			static std::list<CRavidObjectEx*> m_listReceiveMessageObject;

			std::list<CRavidObjectEx*>::iterator m_iterCurrent;
		};
	}
}



