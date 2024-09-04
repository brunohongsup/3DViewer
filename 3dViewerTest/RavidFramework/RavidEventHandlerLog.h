#pragma once

#include "EventHandlerBase.h"

namespace Ravid
{
	namespace Framework
	{
		class CRavidEventHandlerLog : public Ravid::Framework::CEventHandlerBase
		{
			RavidUseDynamicCreation();

		public:
			CRavidEventHandlerLog();
			virtual ~CRavidEventHandlerLog();

			virtual void OnLogMessage(_In_ SRavidLogData* pLogData);
		};
	}
}

