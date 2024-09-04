#pragma once

namespace Ravid
{
	namespace Framework
	{
		enum ESequenceStatus
		{
			ESequenceStatus_Unknown = 0x00,
			ESequenceStatus_Run = 0x01,
			ESequenceStatus_Pause = 0x02,
			ESequenceStatus_Stop = 0x04,
		};

		enum ESequencePriority
		{
			ESequencePriority_Lowest = THREAD_PRIORITY_LOWEST,
			ESequencePriority_BelowNormal = THREAD_PRIORITY_BELOW_NORMAL,
			ESequencePriority_Normal = THREAD_PRIORITY_NORMAL,
			ESequencePriority_Highest = THREAD_PRIORITY_HIGHEST,
			ESequencePriority_AboveNormal = THREAD_PRIORITY_ABOVE_NORMAL,
			ESequencePriority_ErrorReturn = THREAD_PRIORITY_ERROR_RETURN,

			ESequencePriority_TimeCritical = THREAD_BASE_PRIORITY_LOWRT,
			ESequencePriority_Idle = THREAD_BASE_PRIORITY_IDLE,
		};
	}
}

